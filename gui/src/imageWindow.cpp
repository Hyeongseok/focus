/***************************************************************************
 *   Copyright (C) 2006 by UC Davis Stahlberg Laboratory                   *
 *   HStahlberg@ucdavis.edu                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "imageWindow.h"
#include "progressStamps.h"
#include  <QDesktopServices>
#include <iostream>
using namespace std;

imageWindow::imageWindow(confData *conf, QWidget *parent)
: QWidget(parent) {
    data = conf;

    //Setup Leftmost container
    scriptsWidget = new QStackedWidget(this);
    scriptsWidget->setMinimumWidth(180);
    scriptsWidget->setMaximumWidth(300);

    standardScripts = new scriptModule(data, data->getDir("standardScripts"), scriptModule::standard);
    scriptsWidget->addWidget(standardScripts);
    connect(standardScripts, SIGNAL(initialized()), data, SLOT(save()));
    connect(standardScripts, SIGNAL(currentScriptChanged(QModelIndex)), this, SLOT(standardScriptChanged(QModelIndex)));
    connect(standardScripts, SIGNAL(scriptCompleted(QModelIndex)), this, SLOT(standardScriptCompleted(QModelIndex)));
    connect(standardScripts, SIGNAL(runningScriptChanged(QModelIndex)), this, SLOT(standardRunningScriptChanged(QModelIndex)));
    connect(standardScripts, SIGNAL(progress(int)), this, SLOT(setScriptProgress(int)));
    connect(standardScripts, SIGNAL(incrementProgress(int)), this, SLOT(increaseScriptProgress(int)));
    connect(standardScripts, SIGNAL(reload()), this, SLOT(reload()));

    customScripts = new scriptModule(data, data->getDir("customScripts"), scriptModule::custom);
    scriptsWidget->addWidget(customScripts);
    connect(customScripts, SIGNAL(currentScriptChanged(QModelIndex)), this, SLOT(customScriptChanged(QModelIndex)));
    connect(customScripts, SIGNAL(scriptCompleted(QModelIndex)), this, SLOT(customScriptCompleted(QModelIndex)));
    connect(customScripts, SIGNAL(runningScriptChanged(QModelIndex)), this, SLOT(customRunningScriptChanged(QModelIndex)));
    connect(customScripts, SIGNAL(progress(int)), this, SLOT(setScriptProgress(int)));
    connect(customScripts, SIGNAL(incrementProgress(int)), this, SLOT(increaseScriptProgress(int)));
    connect(customScripts, SIGNAL(reload()), this, SLOT(reload()));

    manuals = new QStackedWidget();
    manuals->hide();

    QSplitter* scriptsContainer = new QSplitter(Qt::Vertical);
    scriptsContainer->addWidget(scriptsWidget);
    scriptsContainer->addWidget(manuals);
    scriptsContainer->setStretchFactor(0, 2);
    scriptsContainer->setStretchFactor(1, 1);
    
    
    parameterContainer = setupParameterWindow();
    logWindow = setupLogWindow();
    //warningWindow = new warningBox;
    //logWindow->addWidget(warningWindow);
    
    centralSplitter = new QSplitter(this);
    centralSplitter->setOrientation(Qt::Vertical);

    centralSplitter->addWidget(parameterContainer);
    centralSplitter->addWidget(logWindow);
    centralSplitter->setStretchFactor(0, 1);
    centralSplitter->setStretchFactor(1, 1);

    /*           Results View Information               */

    QSplitter *resultsSplitter = new QSplitter(Qt::Vertical);

    blockContainer *resultsContainer = new blockContainer("Results");
    results = new resultsParser(data, QStringList() << "", resultsParser::results);
    results->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *resultsLoadAction = new QAction("Re-Evaluate Results", results);
    results->addAction(resultsLoadAction);
    connect(resultsLoadAction, SIGNAL(triggered()), this, SLOT(reload()));

    resultsContainer->setMainWidget(results);

    resultsSplitter->addWidget(resultsContainer);
    resultsSplitter->setMinimumWidth(235);

    blockContainer *imagesContainer = new blockContainer("Images");
    connect(imagesContainer, SIGNAL(doubleClicked()), this, SLOT(launchFileBrowser()));

    blockContainer *previewContainer = new blockContainer("Preview");
    previewContainer->setFixedWidth(235);
    preview = new imagePreview(data, "", false, previewContainer);
    connect(preview, SIGNAL(load()), this, SLOT(refresh()));
    previewContainer->setMainWidget(preview);
    
    
    imageParser = new resultsParser(data, QStringList() << "", resultsParser::images);
    connect(imageParser, SIGNAL(imageSelected(const QString &)), preview, SLOT(setImage(const QString&)));
    connect(imageParser, SIGNAL(cellActivated(int, int)), preview, SLOT(launchNavigator()));


    imagesContainer->setMainWidget(imageParser);

    QToolButton* importantSwitch = new QToolButton();
    importantSwitch->setIcon(*(data->getIcon("important")));
    importantSwitch->setToolTip("Important images only");
    importantSwitch->setAutoRaise(false);
    importantSwitch->setCheckable(true);
    connect(importantSwitch, SIGNAL(toggled(bool)), imageParser, SLOT(setImportant(bool)));
    resultsSplitter->addWidget(imagesContainer);

    imagesContainer->setHeaderWidget(importantSwitch);

    QToolButton* showHeaderButton = new QToolButton();
    showHeaderButton->setIcon(*(data->getIcon("info")));
    showHeaderButton->setToolTip("Show Image Header");
    showHeaderButton->setAutoRaise(false);
    showHeaderButton->setCheckable(true);
    showHeaderButton->setChecked(false);
    connect(showHeaderButton, SIGNAL(toggled(bool)), preview, SLOT(showImageHeader(bool)));
    previewContainer->setHeaderWidget(showHeaderButton);
    
    QWidget *rightContainer = new QWidget;
    
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setMargin(0);
    rightLayout->setSpacing(0);
    rightContainer->setLayout(rightLayout);
    rightLayout->addWidget(resultsSplitter);

    centerRightSplitter = new QSplitter(this);
    centerRightSplitter->setOrientation(Qt::Horizontal);
    centerRightSplitter->setHandleWidth(4);
    centerRightSplitter->addWidget(centralSplitter);
    centerRightSplitter->addWidget(rightContainer);
    centerRightSplitter->setStretchFactor(0, 5);
    centerRightSplitter->setStretchFactor(1, 2);

    //Setup status Bar
    progressBar = new QProgressBar(this);
    progressBar->setMaximum(100);
    progressBar->setFixedWidth(300);
    progressBar->setFixedHeight(10);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);

    statusBar = new QStatusBar(this);
    statusBar->setFixedHeight(20);
    statusBar->addPermanentWidget(progressBar);
    
    //Setup the layout and add widgets
    QGridLayout* layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    
    blockContainer* statusParserCont = new blockContainer("Status");
    statusParser = new statusViewer(data->getDir("config") + "/2dx_image/2dx_status.html");
    statusParser->setConf(data);
    statusParser->load();
    statusParserCont->setMainWidget(statusParser);
    
    QWidget *statusContainer = new QWidget();
    statusContainer->setFixedHeight(235);
    
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->setMargin(0);
    statusLayout->setSpacing(0);
    statusContainer->setLayout(statusLayout);
    statusLayout->addWidget(statusParserCont, 1);
    statusLayout->addWidget(previewContainer, 0 , Qt::AlignHCenter | Qt::AlignVCenter);
    
    /*LAYOUT
    * -----------------------------------------------------------
     * | TOOL   | SCRIPTS       | CENTRAL RIGHT SPLITTER
     * | BAR    | WIDGET        | (0, 2, 1 , 1)
     * |(0,0,   | (0,1,2,1)     |
     * |   3,1) |               |
     * |        |               |
     * |        |               |
     * |        |               |
     * |        |               |
     * |        |               |
     * |        |               |__________________________________
     * |        |               | STATUS PARSER (1, 2, 1, 1)
     * |        |_______________|__________________________________
     * |        | STATUSBAR (2, 1, 1,2)
     * -----------------------------------------------------------
     */

    layout->addWidget(setupToolbar(), 0, 0, 3, 1);
    layout->addWidget(scriptsContainer, 0, 1, 2, 1);
    layout->addWidget(centerRightSplitter, 0, 2, 1, 1);
    layout->addWidget(statusContainer, 1, 2, 1, 1);
    layout->addWidget(statusBar, 2, 1, 1, 2);
    
    manuals->hide();
    verbosityControl->setCurrentIndex(1);
    
    
    
    //Just to set correct siezs
    maximizeLogWindow(false);
    maximizeParameterWindow(false);
    
    setStandardMode();
}

void imageWindow::bridgeScriptLogConnection(bool bridge) {
    if (bridge) {
        connect(standardScripts, SIGNAL(standardOut(const QStringList &)), logViewer, SLOT(insertText(const QStringList &)));
        connect(standardScripts, SIGNAL(standardError(const QByteArray &)), logViewer, SLOT(insertError(const QByteArray &)));
        connect(customScripts, SIGNAL(standardOut(const QStringList &)), logViewer, SLOT(insertText(const QStringList &)));
        connect(customScripts, SIGNAL(standardError(const QByteArray &)), logViewer, SLOT(insertError(const QByteArray &)));
        connect(standardScripts, SIGNAL(scriptLaunched()), logViewer, SLOT(clear()));
        connect(customScripts, SIGNAL(scriptLaunched()), logViewer, SLOT(clear()));
    } else {
        disconnect(standardScripts, SIGNAL(standardOut(const QStringList &)), logViewer, SLOT(insertText(const QStringList &)));
        disconnect(standardScripts, SIGNAL(standardError(const QByteArray &)), logViewer, SLOT(insertError(const QByteArray &)));
        disconnect(customScripts, SIGNAL(standardOut(const QStringList &)), logViewer, SLOT(insertText(const QStringList &)));
        disconnect(customScripts, SIGNAL(standardError(const QByteArray &)), logViewer, SLOT(insertError(const QByteArray &)));
        disconnect(standardScripts, SIGNAL(scriptLaunched()), logViewer, SLOT(clear()));
        disconnect(customScripts, SIGNAL(scriptLaunched()), logViewer, SLOT(clear()));
    }
}

blockContainer* imageWindow::setupLogWindow() {
    blockContainer *logWindow = new blockContainer("Output (Double click for logbrowser)", this);
    logWindow->setMinimumWidth(400);
    logWindow->setMinimumHeight(200);

    //Setup Log Viewer
    logViewer = new LogViewer("Standard Output", NULL);
    bridgeScriptLogConnection(true);

    //Setup Verbosity control combo box
    verbosityControl = new QComboBox(this);
    verbosityControl->addItems(QStringList() << "Silent" << "Low" << "Moderate" << "Highest");

    connect(verbosityControl, SIGNAL(currentIndexChanged(int)), logViewer, SLOT(load(int)));
    connect(verbosityControl, SIGNAL(currentIndexChanged(int)), standardScripts, SLOT(setVerbosity(int)));
    connect(verbosityControl, SIGNAL(currentIndexChanged(int)), customScripts, SLOT(setVerbosity(int)));

    //Setup History View
    QToolButton* historyButton = new QToolButton();
    historyButton->setFixedSize(QSize(20, 20));
    historyButton->setIcon(*(data->getIcon("info")));
    historyButton->setToolTip("Processing history toggle");
    historyButton->setAutoRaise(false);
    historyButton->setCheckable(true);
    connect(historyButton, SIGNAL(toggled(bool)), this, SLOT(toggleHistoryView(bool)));

    //Setup Maximize tool button
    QToolButton* maximizeLogWindow = new QToolButton();
    maximizeLogWindow->setFixedSize(QSize(20, 20));
    maximizeLogWindow->setIcon(*(data->getIcon("maximize")));
    maximizeLogWindow->setToolTip("Maximize output view");
    maximizeLogWindow->setAutoRaise(false);
    maximizeLogWindow->setCheckable(true);

    connect(maximizeLogWindow, SIGNAL(toggled(bool)), this, SLOT(maximizeLogWindow(bool)));

    QWidget* verbosityControlWidget = new QWidget();
    QGridLayout* verbosityControlLayout = new QGridLayout(verbosityControlWidget);
    verbosityControlLayout->setMargin(0);
    verbosityControlLayout->setSpacing(0);
    verbosityControlWidget->setLayout(verbosityControlLayout);

    verbosityControlLayout->addWidget(new QLabel("Verbosity Level: "), 0, 0);
    verbosityControlLayout->addWidget(verbosityControl, 0, 1, 1, 1, Qt::AlignVCenter);
    verbosityControlLayout->addItem(new QSpacerItem(3, 3), 0, 2);
    verbosityControlLayout->addWidget(historyButton, 0, 3);
    verbosityControlLayout->addItem(new QSpacerItem(3, 3), 0, 4);
    verbosityControlLayout->addWidget(maximizeLogWindow, 0, 5);

    //Setup the window and add widgets

    logWindow->setMainWidget(logViewer);
    logWindow->setHeaderWidget(verbosityControlWidget);

    connect(logWindow, SIGNAL(doubleClicked()), this, SLOT(launchLogBrowser()));

    return logWindow;

}

blockContainer* imageWindow::setupParameterWindow() {
    localParameters = new resizeableStackedWidget(this);

    parameters = new confInterface(data, "");

    parametersWidget = new QWidget();
    QVBoxLayout *parameterLayout = new QVBoxLayout();
    parameterLayout->setMargin(0);
    parameterLayout->setSpacing(0);
    parametersWidget->setLayout(parameterLayout);
    parameterLayout->addWidget(localParameters);
    parameterLayout->addWidget(parameters);
    parameterLayout->setStretchFactor(localParameters, 1);
    parameterLayout->setStretchFactor(parameters, 100);

    QScrollArea *window = new QScrollArea(this);
    window->setWidgetResizable(true);
    window->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    window->setWidget(parametersWidget);


    //Setup Verbosity control combo box
    userLevelButtons = new QComboBox(this);
    userLevelButtons->addItems(QStringList() << "Simplified" << "Advanced");
    connect(userLevelButtons, SIGNAL(currentIndexChanged(int)), parameters, SLOT(setSelectionUserLevel(int)));

    //Setup Maximize tool button
    QToolButton* maximizeParameterWin = new QToolButton();
    maximizeParameterWin->setFixedSize(QSize(20, 20));
    maximizeParameterWin->setIcon(*(data->getIcon("maximize")));
    maximizeParameterWin->setToolTip("Maximize parameter view");
    maximizeParameterWin->setAutoRaise(false);
    maximizeParameterWin->setCheckable(true);

    connect(maximizeParameterWin, SIGNAL(toggled(bool)), this, SLOT(maximizeParameterWindow(bool)));

    QWidget* parameterLevelWidget = new QWidget();
    QGridLayout* parameterLevelLayout = new QGridLayout(parameterLevelWidget);
    parameterLevelLayout->setMargin(0);
    parameterLevelLayout->setSpacing(0);
    parameterLevelWidget->setLayout(parameterLevelLayout);

    parameterLevelLayout->addWidget(new QLabel("Level: "), 0, 0);
    parameterLevelLayout->addWidget(userLevelButtons, 0, 1, 1, 1, Qt::AlignVCenter);
    parameterLevelLayout->addItem(new QSpacerItem(3, 3), 0, 2);
    parameterLevelLayout->addWidget(maximizeParameterWin, 0, 3);

    //Setup the window and add widgets
    blockContainer* parameterContainer = new blockContainer("Setup");
    parameterContainer->setMinimumWidth(400);
    parameterContainer->setMinimumHeight(200);
    parameterContainer->setMainWidget(window);
    parameterContainer->setHeaderWidget(parameterLevelWidget);

    return parameterContainer;
}

QToolBar* imageWindow::setupToolbar() {
    QToolBar* scriptsToolBar = new QToolBar("Choose Mode", this);
    scriptsToolBar->setOrientation(Qt::Vertical);

    showStandardScripts = new QToolButton(scriptsToolBar);
    showStandardScripts->setIcon(*(data->getIcon("standard")));
    showStandardScripts->setFixedSize(QSize(64, 64));
    showStandardScripts->setText("Standard");
    showStandardScripts->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    showStandardScripts->setCheckable(true);
    connect(showStandardScripts, SIGNAL(clicked()), this, SLOT(setStandardMode()));

    showCustomScripts = new QToolButton(scriptsToolBar);
    showCustomScripts->setIcon(*(data->getIcon("custom")));
    showCustomScripts->setFixedSize(QSize(64, 64));
    showCustomScripts->setText("Custom");
    showCustomScripts->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    showCustomScripts->setCheckable(true);
    connect(showCustomScripts, SIGNAL(clicked()), this, SLOT(setCustomMode()));

    scriptsToolBar->addWidget(showStandardScripts);
    scriptsToolBar->addWidget(showCustomScripts);

    return scriptsToolBar;

}

void imageWindow::scriptChanged(scriptModule *module, QModelIndex index) {
    int uid = index.data(Qt::UserRole).toUInt();
    currentResults = module->resultsFile(index);

    if ((module == customScripts && !standardScripts->isRunning()) || (module == standardScripts && !customScripts->isRunning())) updateStatusMessage(module->title(index));

    if (localIndex[uid] == 0 && module->conf(index)->size() != 0) {
        confInterface *local = new confInterface(module->conf(index), "");
        connect(this, SIGNAL(fontInfoUpdated()), local, SLOT(updateFontInfo()));
        localIndex[uid] = localParameters->addWidget(local) + 1;
    }

    if (manualIndex[uid] == 0 && !module->conf(index)->manual().isEmpty())
        manualIndex[uid] = manuals->addWidget(new confManual(data, module->conf(index))) + 1;

    if (localIndex[uid] - 1 < 0)
        localParameters->hide();
    else {
        localParameters->show();
        localParameters->setCurrentIndex(localIndex[uid] - 1);
    }

    manuals->setCurrentIndex(manualIndex[uid] - 1);

    parameters->select(module->displayedVariables(index));
    currentLog = module->logFile(index);
    if (!visible["historyview"]) logViewer->loadLogFile(currentLog);
    results->setResult(module->resultsFile(index));
    imageParser->setResult(module->resultsFile(index));
    //warningWindow->load(module->resultsFile(index));
    parametersWidget->update();
}

void imageWindow::standardScriptChanged(QModelIndex index) {
    scriptChanged(standardScripts, index);
}

void imageWindow::customScriptChanged(QModelIndex index) {
    scriptChanged(customScripts, index);
}

bool imageWindow::parseResults(confData *conf, const QString &results) {
    currentResults = results;
    return scriptParser::parseResults(conf, results);
}

bool imageWindow::parseResults() {
    return parseResults(data, currentResults);
}

void imageWindow::scriptCompleted(scriptModule *module, QModelIndex index) {
    if (!module->isRunning())
        parseResults(data, module->resultsFile(index));
    //  results->setResult(module->resultsFile(index));
    imageParser->setResult(module->resultsFile(index));
    parameters->load();
    //statusParser->load();

    emit scriptCompletedSignal();
}

void imageWindow::standardScriptCompleted(QModelIndex index) {
    scriptCompleted(standardScripts, index);
}

void imageWindow::customScriptCompleted(QModelIndex index) {
    scriptCompleted(customScripts, index);
}

void imageWindow::runningScriptChanged(scriptModule *module, QModelIndex index) {
    updateStatusMessage(module->title(index));
}

void imageWindow::customRunningScriptChanged(QModelIndex index) {
    runningScriptChanged(customScripts, index);
}

void imageWindow::standardRunningScriptChanged(QModelIndex index) {
    runningScriptChanged(standardScripts, index);
}

void imageWindow::setStandardMode() {
    showStandardScripts->setChecked(true);
    showCustomScripts->setChecked(false);
    scriptsWidget->setCurrentWidget(standardScripts);
    standardScripts->focusWidget();
}

void imageWindow::setCustomMode() {
    showStandardScripts->setChecked(false);
    showCustomScripts->setChecked(true);
    scriptsWidget->setCurrentWidget(customScripts);
    customScripts->focusWidget();
}

void imageWindow::maximizeLogWindow(bool maximize) {
    if (maximize) {
        centralSplitter->setSizes(QList<int>() << 0 << 1);
        centerRightSplitter->setSizes(QList<int>() << 1 << 0);
    } else {
        centralSplitter->setSizes(QList<int>() << 1 << 1);
        centerRightSplitter->setSizes(QList<int>() << 5 << 2);
    }
}

void imageWindow::maximizeParameterWindow(bool maximize) {
    
    if (maximize) {
        centralSplitter->setSizes(QList<int>() << 1 << 0);
        centerRightSplitter->setSizes(QList<int>() << 1 << 0);
    } else {
        centralSplitter->setSizes(QList<int>() << 1 << 1);
        
        centerRightSplitter->setSizes(QList<int>() << 5 << 2);
    }
}

void imageWindow::execute(bool halt) {
    scriptModule* module = (scriptModule*) scriptsWidget->currentWidget();
    if (module->type() == scriptModule::standard) {
        standardScripts->execute(halt);
    }
    if (module->type() == scriptModule::custom) {
        customScripts->execute(halt);
    }
}

void imageWindow::reload() {
    parseResults();
    refresh();
}

void imageWindow::refresh() {
    parameters->load();
    //statusParser->load();
    results->load();
    imageParser->load();
    //statusParser->load();
}

void imageWindow::updateStatusMessage(const QString& message) {
    progressBar->update();
    statusBar->showMessage(message);
}

void imageWindow::increaseScriptProgress(int increament) {
    if (progressBar->value() + increament <= progressBar->maximum())
        progressBar->setValue(progressBar->value() + increament);
    else
        progressBar->setValue(progressBar->maximum());
}

void imageWindow::setScriptProgress(int progress) {
    progressBar->setValue(progress);
}

void imageWindow::showManual(bool show) {
    if (show) {
        manuals->show();
    } else {
        manuals->hide();
    }
}

void imageWindow::revert() {
    data->reload();
    parameters->load();
    //statusParser->load();
}

void imageWindow::updateFontInfo() {
    parameters->updateFontInfo();
    logViewer->updateFontInfo();
    results->updateFontInfo();
    imageParser->updateFontInfo();
    emit fontInfoUpdated();
}

void imageWindow::launchLogBrowser() {
    QProcess::startDetached(data->getApp("logBrowser") + " " + logViewer->getLogFile());
}

void imageWindow::launchFileBrowser() {
    QString path = QDir::toNativeSeparators(data->getDir("working"));
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

void imageWindow::toggleHistoryView(bool show) {
    visible["historyview"] = show;
    if (show) {
        bridgeScriptLogConnection(false);
        logViewer->loadLogFile(data->getDir("working") + "/" + "History.dat");
        logWindow->setHeaderTitle("Processing history");
    } else {
        logViewer->loadLogFile(currentLog);
        bridgeScriptLogConnection(true);
        logWindow->setHeaderTitle("Output (Double click for logbrowser)");
    }
    verbosityControl->setCurrentIndex(verbosityControl->currentIndex());
}

void imageWindow::useNewViewer(bool enable) {
    preview->enableNewViewer(enable);
}
