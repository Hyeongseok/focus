file(GLOB FSPLIT_FILES *.f )
message(STATUS ${FSPLIT_FILES})
file(WRITE ${FSPLIT_OUTFILE} "" )
foreach(FFILE ${FSPLIT_FILES})
	file(APPEND ${FSPLIT_OUTFILE} "${FFILE}\n")
endforeach(FFILE ${FSPLIT_FILES})
