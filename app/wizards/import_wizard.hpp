/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   import_wizard.hpp
 * Author: biyanin
 *
 * Created on June 10, 2016, 6:16 PM
 */

#ifndef IMPORT_WIZARD_HPP
#define IMPORT_WIZARD_HPP

#include <QWizard>
#include <QString>

#include "import_result.hpp"
#include "import_images.hpp"
#include "label_page.hpp"

namespace tdx {

    namespace app {

        namespace wizard {

            class ImportWizard : public QWizard {
                
                Q_OBJECT
                
            public:

                ImportWizard(QWidget *parent)
                : QWizard(parent) {
                    
                    QWizardPage* introPage = new LabelWizardPage(introText());
                    introPage->setTitle("Introduction");
                    introPage->setSubTitle("This wizard will guide you to import images");
                    
                    QWizardPage* conclusionPage = new LabelWizardPage(conclusionText());
                    conclusionPage->setTitle("Conclusion");
                    
                    addPage(introPage);
                    addPage(new ImportImagesWizardPage);
                    addPage(new ImportResultWizardPage);
                    addPage(conclusionPage);

                    //setWizardStyle(QWizard::MacStyle);
                    setWindowTitle(tr("Import images and movies"));
                }
                
            private:
                QString introText() {
                    return (QString("Before starting please ensure that the averaged " + 
                            QString("stacks and the movie files to be imported are in separate ")+
                            QString("folders. The corresponding images and the movies should ") +
                            QString("have same name.")));
                }
                
                QString conclusionText() {
                    return QString("The process was successfully completed");
                }



            };
        }
    }
}

#endif /* IMPORT_WIZARD_HPP */

