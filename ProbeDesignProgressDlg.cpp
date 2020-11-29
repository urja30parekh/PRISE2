#include "ProbeDesignProgressDlg.h"

ProbeDesignProgressDlg::ProbeDesignProgressDlg()
{

    disconnect(this, SIGNAL(canceled()),this, SLOT(cancel()));
    connect(this, SIGNAL(canceled()),this, SLOT(confirmCancel()));
    QString labelText = "Searching for probe candidates in target sequences.<p>"
                        "Filtering probe candidates according to user specifications.<p>"
                        "Searching probes pairs in target sequences.<p>"
                        "Searching probes pairs in non-target sequences.<p>"
                        "Preparing output.";
    this->setLabelText(labelText);
    this->setRange(0,1);
    this->setValue(0);
}
void ProbeDesignProgressDlg::computation(int part)
{
    QString labelText;
    QString type, type_ext;
    if (task == 0){
        type = "primer";
        type_ext =" pairs";
    }
    else if(task == 1){
        type = "probe";
        type_ext ="";
    }
    else{
        type  = "primer/probe";
    }

    switch(part){
    case 1:
        labelText ="<div align=\"left\">"
                            "1.<span style=\" color:#ff0000;\">Searching for "+type+" candidates in target sequences.</span><p>"
                            "2.Filtering "+type+" candidates according to user specifications.<p>"
                            "3.Searching "+type+type_ext+" in target sequences.<p>"
                            "4.Searching "+type+type_ext+" in non-target sequences.<p>"
                            "5.Preparing output."
                            "</div>";
        break;
    case 2:
        labelText ="<div align=\"left\">"
                            "1.Searching for "+type+" candidates in target sequences<p>"
                            "2.<span style=\" color:#ff0000;\">Filtering primer candidates according to user specifications</span><p>"
                            "3.Searching "+type+type_ext+" in target sequences<p>"
                            "4.Searching "+type+type_ext+" in non-target sequences<p>"
                            "5.Preparing output"
                            "</div>";
        break;
    case 3:
        labelText ="<div align=\"left\">"
                            "1.Searching for "+type+" candidates in target sequences.<p>"
                            "2.Filtering primer candidates according to user specifications.<p>"
                            "3.<span style=\" color:#ff0000;\">Searching "+type+type_ext+" in target sequences.</span><p>"
                            "4.Searching "+type+type_ext+" in non-target sequences.<p>"
                            "5.Preparing output."
                            "</div>";
        break;
    case 4:
        labelText ="<div align=\"left\">"
                            "1.Searching for "+type+" candidates in target sequences.<p>"
                            "2.Filtering primer candidates according to user specifications.<p>"
                            "3.Searching "+type+type_ext+" in target sequences.<p>"
                            "4.<span style=\" color:#ff0000;\">Searching "+type+type_ext+" in non-target sequences.</span><p>"
                            "5.Preparing output."
                            "</div>";
        break;
    default:
        labelText ="<div align=\"left\">"
                            "1.Searching for "+type+" candidates in target sequences.<p>"
                            "2.Filtering primer candidates according to user specifications.<p>"
                            "3.Searching "+type+type_ext+" in target sequences.<p>"
                            "4.Searching "+type+type_ext+" in non-target sequences.<p>"
                            "5.<span style=\" color:#ff0000;\">Preparing output.</span>"
                            "</div>";
        break;
    }
    this->setLabelText(labelText);
}


void ProbeDesignProgressDlg::confirmCancel()
{
    MyConfirmDlg* cDlg = new MyConfirmDlg("Do you really want to terminate the computation?\n\n\n\n"
                                          "Click OK to terminate the program, Cancel to resume",this);
    if(cDlg->exec()){
        delete cDlg;
        ((QProgressDialog*)this)->cancel();
        return;
    }else{
        delete cDlg;
    }
}
