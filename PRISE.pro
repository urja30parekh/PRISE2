# -------------------------------------------------
# Project created by QtCreator 2010-03-30T15:23:51
# -------------------------------------------------

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7 # this is to handle earlier versions of MAC OS
win32 {
    #TODO: perhaps there's a better way to do this?
} macx {

} unix:!macx {

}

QT += network widgets

TARGET = PRISE2
TEMPLATE = app
SOURCES += main.cpp \
    prise.cpp \
    SeqReportWindow.cpp \
    LoadSeedHitTableDlg.cpp \
    SeqAlignSettingDlg.cpp \
    tools.cpp \
    SeqAlign.cpp \
    MessageDlg.cpp \
    MyConfirmDlg.cpp \
    SeqMarkUnmarkDlg.cpp \
    SeqFindDlg.cpp \
    PRISESetting.cpp \
    SeqDiffWindow.cpp \
    SeqAlignShowWindow.cpp \
    LoadOrNewPage.cpp \
    pSeqsInputPage.cpp \
    pGetCandidatePage.cpp \
    pSettingStartPage.cpp \
    PrimerSettingPropertyPage.cpp \
    PrimerSettingSlctvtBscPage.cpp \
    PrimerSettingSlctvtAdvIPage.cpp \
    PrimerSettingSlctvtAdvIIPage.cpp \
    Data.cpp \
    Designer.cpp \
    PrimerSettingReportDlg.cpp \
    PrimerQuickSearch.cpp \
    PrimerReportWindow.cpp \
    PrimerDispListDlg.cpp \
    PrimerSearch.cpp \
    PrimerSaveListDlg.cpp \
    PrimerSaveExcelDlg.cpp \
    PrimerColumnDlg.cpp \
    PrimerSortDlg.cpp \
    PrimerDeleteCondDlg.cpp \
    AnnealInfoDlg.cpp \
    PrimerDesignProgressDlg.cpp \
    MyProgressDlg.cpp \
    DesignWizard.cpp \
    ProbeGetCandidatePage.cpp \
    ProbeSettingPropertyPage.cpp \
    ProbeSettingSlctvtAdvIPage.cpp \
    ProbeSettingSlctvtAdvIIPage.cpp \
    ProbeQuickSearch.cpp \
    TripleReportWindow.cpp \
    dialog.cpp \
    ProbeDesignProgressDlg.cpp \
    TripleSaveListDlg.cpp \
    TripleSaveExcelDlg.cpp \
    TripleColumnDlg.cpp \
    TripleDeleteCondDlg.cpp \
    Probe.cpp \
    PrimerPair.cpp \
    Triple.cpp \
    TripleSortDlg.cpp \
    DataTools.cpp \
    ProbeSettingReportDlg.cpp \
    TripleReportFrame.cpp \
    InstructionDlg.cpp \
    ProbeSettingSlctvtBscPage.cpp \
    PrimerItemDelegate.cpp \
    ProbeItemDelegate.cpp \
    TripleDispListDlg.cpp \
    SeqTableView.cpp \
    blastwizard.cpp \
    localorwebpage.cpp \
    blastparapage.cpp \
    blastquerypage.cpp \
    blastresultdialog.cpp \
    seqreportmodel.cpp
HEADERS += prise.h \
    SeqReportWindow.h \
    LoadSeedHitTableDlg.h \
    SeqAlignSettingDlg.h \
    SequenceList.h \
    tools.h \
    GlobalDef.h \
    SeqAlign.h \
    MessageDlg.h \
    MyConfirmDlg.h \
    SeqMarkUnmarkDlg.h \
    SeqFindDlg.h \
    PRISESetting.h \
    SeqDiffWindow.h \
    SeqAlignShowWindow.h \
    sorting.h \
    LoadOrNewPage.h \
    pSeqsInputPage.h \
    pGetCandidatePage.h \
    pSettingStartPage.h \
    PrimerSettingPropertyPage.h \
    PrimerSettingSlctvtBscPage.h \
    PrimerSettingSlctvtAdvIPage.h \
    PrimerSettingSlctvtAdvIIPage.h \
    DataTools.h \
    Data.h \
    Designer.h \
    PrimerSettingReportDlg.h \
    gmp.h \
    PrimerQuickSearch.h \
    PrimerPair.h \
    PrimerReportWindow.h \
    PrimerDispListDlg.h \
    PrimerSearch.h \
    PrimerSaveListDlg.h \
    PrimerSaveExcelDlg.h \
    PrimerColumnDlg.h \
    PrimerSortDlg.h \
    PrimerDeleteCondDlg.h \
    AnnealInfoDlg.h \
    PrimerDesignProgressDlg.h \
    MyProgressDlg.h \
    DesignWizard.h \
    ProbeGetCandidatePage.h \
    WizardPageOrder.h \
    ProbeSettingPropertyPage.h \
    ProbeSettingSlctvtAdvIPage.h \
    ProbeSettingSlctvtAdvIIPage.h \
    Probe.h \
    ProbeQuickSearch.h \
    TripleReportWindow.h \
    dialog.h \
    ProbeDesignProgressDlg.h \
    TripleSaveListDlg.h \
    TripleSaveExcelDlg.h \
    TripleColumnDlg.h \
    TripleDeleteCondDlg.h \
    DataTools.h \
    Triple.h \
    TripleSortDlg.h \
    ProbeSettingReportDlg.h \
    TripleReportFrame.h \
    InstructionDlg.h \
    ProbeSettingSlctvtBscPage.h \
    PrimerItemDelegate.h \
    ProbeItemDelegate.h \
    TripleDispListDlg.h \
    SeqTableView.h \
    blastwizard.h \
    localorwebpage.h \
    blastparapage.h \
    blastquerypage.h \
    blastresultdialog.h \
    seqreportmodel.h
FORMS += prise.ui \
    SeqReportWindow.ui \
    LoadSeedHitTableDlg.ui \
    SeqAlignSettingDlg.ui \
    MessageDlg.ui \
    MyConfirmDlg.ui \
    SeqMarkUnmarkDlg.ui \
    SeqFindDlg.ui \
    SeqDiffWindow.ui \
    SeqAlignShowWindow.ui \
    LoadOrNewPage.ui \
    pSeqsInputPage.ui \
    pGetCandidatePage.ui \
    pSettingStartPage.ui \
    PrimerSettingPropertyPage.ui \
    PrimerSettingSlctvtBscPage.ui \
    PrimerSettingSlctvtAdvIPage.ui \
    PrimerSettingSlctvtAdvIIPage.ui \
    PrimerSettingReportDlg.ui \
    PrimerReportWindow.ui \
    PrimerDispListDlg.ui \
    PrimerSaveListDlg.ui \
    PrimerSaveExcelDlg.ui \
    PrimerColumnDlg.ui \
    PrimerSortDlg.ui \
    PrimerDeleteCondDlg.ui \
    AnnealInfoDlg.ui \
    DesignWizard.ui \
    ProbeGetCandidatePage.ui \
    ProbeSettingPropertyPage.ui \
    ProbeSettingSlctvtAdvIPage.ui \
    ProbeSettingSlctvtAdvIIPage.ui \
    dialog.ui \
    TripleReportWindow.ui \
    TripleSaveListDlg.ui \
    TripleSaveExcelDlg.ui \
    TripleColumnDlg.ui \
    TripleDeleteCondDlg.ui \
    TripleSortDlg.ui \
    ProbeSettingReportDlg.ui \
    TripleReportFrame.ui \
    InstructionDlg.ui \
    ProbeSettingSlctvtBscPage.ui \
    TripleDispListDlg.ui \
    blastwizard.ui \
    localorwebpage.ui \
    blastparapage.ui \
    blastquerypage.ui \
    blastresultdialog.ui
INCLUDEPATH += "lib/ttmath-0.9.1"
CONFIG += exceptions


CONFIG += C++11

#QMAKE_CXXFLAGS += -std=c++11
#QMAKE_CXXFLAGS += -mdynamic-no-pic -fno-pic

RESOURCES += prise.qrc
