#include "ProbeSettingSlctvtAdvIPage.h"
#include "ui_ProbeSettingSlctvtAdvIPage.h"

ProbeSettingSlctvtAdvIPage::ProbeSettingSlctvtAdvIPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProbeSettingSlctvtAdvIPage)
{
    ui->setupUi(this);
    //ui->radioButton_default->setChecked(!showSimMeasureAdv);
    //ui->radioButton_showMtx->setChecked(showSimMeasureAdv);
    //this->setFinalPage(!showSimMeasureAdv);
}

ProbeSettingSlctvtAdvIPage::~ProbeSettingSlctvtAdvIPage()
{
    delete ui;
}

void ProbeSettingSlctvtAdvIPage::initializePage()
{
#ifdef MAC
    this->resize(900, 410);
#else
    this->resize(900, 410);
#endif

    QWizard* p = this->wizard();
    prevWidth = p->width();
    prevHeight = p->height();

#ifdef MAC
    p->resize(width()+200, height()+600);
#else
    p->resize(width()+200, height()+200);
#endif


    ui->lineEdit_1->setText(QString("%1").arg(probePosWtTar[14]));
    ui->lineEdit_2->setText(QString("%1").arg(probePosWtTar[13]));
    ui->lineEdit_3->setText(QString("%1").arg(probePosWtTar[12]));
    ui->lineEdit_4->setText(QString("%1").arg(probePosWtTar[11]));
    ui->lineEdit_5->setText(QString("%1").arg(probePosWtTar[10]));
    ui->lineEdit_6->setText(QString("%1").arg(probePosWtTar[9]));
    ui->lineEdit_7->setText(QString("%1").arg(probePosWtTar[8]));
    ui->lineEdit_8->setText(QString("%1").arg(probePosWtTar[7]));
    ui->lineEdit_9->setText(QString("%1").arg(probePosWtTar[6]));
    ui->lineEdit_10->setText(QString("%1").arg(probePosWtTar[5]));
    ui->lineEdit_11->setText(QString("%1").arg(probePosWtTar[4]));
    ui->lineEdit_12->setText(QString("%1").arg(probePosWtTar[3]));
    ui->lineEdit_13->setText(QString("%1").arg(probePosWtTar[2]));
    ui->lineEdit_14->setText(QString("%1").arg(probePosWtTar[1]));
    ui->lineEdit_15->setText(QString("%1").arg(probePosWtTar[0]));
    ui->lineEdit_16->setText(QString("%1").arg(probePosWtTar[0]));
    ui->lineEdit_17->setText(QString("%1").arg(probePosWtTar[1]));
    ui->lineEdit_18->setText(QString("%1").arg(probePosWtTar[2]));
    ui->lineEdit_19->setText(QString("%1").arg(probePosWtTar[3]));
    ui->lineEdit_20->setText(QString("%1").arg(probePosWtTar[4]));
    ui->lineEdit_21->setText(QString("%1").arg(probePosWtTar[5]));
    ui->lineEdit_22->setText(QString("%1").arg(probePosWtTar[6]));
    ui->lineEdit_23->setText(QString("%1").arg(probePosWtTar[7]));
    ui->lineEdit_24->setText(QString("%1").arg(probePosWtTar[8]));
    ui->lineEdit_25->setText(QString("%1").arg(probePosWtTar[9]));
    ui->lineEdit_26->setText(QString("%1").arg(probePosWtTar[10]));
    ui->lineEdit_27->setText(QString("%1").arg(probePosWtTar[11]));
    ui->lineEdit_28->setText(QString("%1").arg(probePosWtTar[12]));
    ui->lineEdit_29->setText(QString("%1").arg(probePosWtTar[13]));
    ui->lineEdit_30->setText(QString("%1").arg(probePosWtTar[14]));

    ui->lineEdit_n1->setText(QString("%1").arg(probePosWtNonTar[14]));
    ui->lineEdit_n2->setText(QString("%1").arg(probePosWtNonTar[13]));
    ui->lineEdit_n3->setText(QString("%1").arg(probePosWtNonTar[12]));
    ui->lineEdit_n4->setText(QString("%1").arg(probePosWtNonTar[11]));
    ui->lineEdit_n5->setText(QString("%1").arg(probePosWtNonTar[10]));
    ui->lineEdit_n6->setText(QString("%1").arg(probePosWtNonTar[9]));
    ui->lineEdit_n7->setText(QString("%1").arg(probePosWtNonTar[8]));
    ui->lineEdit_n8->setText(QString("%1").arg(probePosWtNonTar[7]));
    ui->lineEdit_n9->setText(QString("%1").arg(probePosWtNonTar[6]));
    ui->lineEdit_n10->setText(QString("%1").arg(probePosWtNonTar[5]));
    ui->lineEdit_n11->setText(QString("%1").arg(probePosWtNonTar[4]));
    ui->lineEdit_n12->setText(QString("%1").arg(probePosWtNonTar[3]));
    ui->lineEdit_n13->setText(QString("%1").arg(probePosWtNonTar[2]));
    ui->lineEdit_n14->setText(QString("%1").arg(probePosWtNonTar[1]));
    ui->lineEdit_n15->setText(QString("%1").arg(probePosWtNonTar[0]));
    ui->lineEdit_n16->setText(QString("%1").arg(probePosWtNonTar[0]));
    ui->lineEdit_n17->setText(QString("%1").arg(probePosWtNonTar[1]));
    ui->lineEdit_n18->setText(QString("%1").arg(probePosWtNonTar[2]));
    ui->lineEdit_n19->setText(QString("%1").arg(probePosWtNonTar[3]));
    ui->lineEdit_n20->setText(QString("%1").arg(probePosWtNonTar[4]));
    ui->lineEdit_n21->setText(QString("%1").arg(probePosWtNonTar[5]));
    ui->lineEdit_n22->setText(QString("%1").arg(probePosWtNonTar[6]));
    ui->lineEdit_n23->setText(QString("%1").arg(probePosWtNonTar[7]));
    ui->lineEdit_n24->setText(QString("%1").arg(probePosWtNonTar[8]));
    ui->lineEdit_n25->setText(QString("%1").arg(probePosWtNonTar[9]));
    ui->lineEdit_n26->setText(QString("%1").arg(probePosWtNonTar[10]));
    ui->lineEdit_n27->setText(QString("%1").arg(probePosWtNonTar[11]));
    ui->lineEdit_n28->setText(QString("%1").arg(probePosWtNonTar[12]));
    ui->lineEdit_n29->setText(QString("%1").arg(probePosWtNonTar[13]));
    ui->lineEdit_n30->setText(QString("%1").arg(probePosWtNonTar[14]));
    }

void ProbeSettingSlctvtAdvIPage::cleanupPage()
{
    QWizard* p = wizard();
    p->resize(prevWidth, prevHeight);
}

void ProbeSettingSlctvtAdvIPage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int ProbeSettingSlctvtAdvIPage::nextId() const
{
    if (ui->radioButton_default->isChecked()) {
        return -1;
    } else {
        return Page_probeSettingAdvII;
    }
}

bool ProbeSettingSlctvtAdvIPage::validatePage()
{
    //showSimMeasureAdv = ui->radioButton_showMtx->isChecked();
    bool b;
    probePosWtTar[0] = ui->lineEdit_15->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[1] = ui->lineEdit_14->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[2] = ui->lineEdit_13->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[3] = ui->lineEdit_12->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[4] = ui->lineEdit_11->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[5] = ui->lineEdit_10->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[6] = ui->lineEdit_9->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[7] = ui->lineEdit_8->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[8] = ui->lineEdit_7->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[9] = ui->lineEdit_6->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[10] = ui->lineEdit_5->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[11] = ui->lineEdit_4->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[12] = ui->lineEdit_3->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[13] = ui->lineEdit_2->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtTar[14] = ui->lineEdit_1->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    for( int i = 14; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtTar[i] = probePosWtTar[14];


    probePosWtNonTar[0] = ui->lineEdit_n15->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[1] = ui->lineEdit_n14->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[2] = ui->lineEdit_n13->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[3] = ui->lineEdit_n12->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[4] = ui->lineEdit_n11->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[5] = ui->lineEdit_n10->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[6] = ui->lineEdit_n9->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[7] = ui->lineEdit_n8->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[8] = ui->lineEdit_n7->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[9] = ui->lineEdit_n6->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[10] = ui->lineEdit_n5->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[11] = ui->lineEdit_n4->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[12] = ui->lineEdit_n3->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[13] = ui->lineEdit_n2->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    probePosWtNonTar[14] = ui->lineEdit_n1->text().toInt(&b);
    if(!b){
        QMessageBox::about(this, "Error value","Please make sure you input valid number.");
        return false;
    }
    for( int i = 14; i < MAX_PROBE_LEN/2+1; i++ )
        probePosWtNonTar[i] = probePosWtNonTar[14];

    return ((QWizardPage)this).validatePage();
}

void ProbeSettingSlctvtAdvIPage::onCurrentSetting()
{
    //this->setFinalPage(ui->radioButton_default->isChecked());

    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}

void ProbeSettingSlctvtAdvIPage::onShowMtx()
{
    //this->setFinalPage(!ui->radioButton_showMtx->isChecked());

    if (this->nextId() == -1)
        setFinalPage(true);
    else
        setFinalPage(false);
}

void ProbeSettingSlctvtAdvIPage::onEditTar_1()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_1   ->text();
    ui->lineEdit_30->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_1->setText("");
            ui->lineEdit_30->setText("");
        }
        return;
    }

    strB = ui->lineEdit_2->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_2->setText(strA);


}
void ProbeSettingSlctvtAdvIPage::onEditTar_2()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_2->text();
    ui->lineEdit_29->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_2->setText("");
            ui->lineEdit_29->setText("");
        }
        return;
    }

    strB = ui->lineEdit_1->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_1->setText(strA);

    strB = ui->lineEdit_3->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_3->setText(strA);
}

void ProbeSettingSlctvtAdvIPage::onEditTar_3()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_3->text();
    ui->lineEdit_28->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_3->setText("");
            ui->lineEdit_28->setText("");
        }
        return;
    }

    strB = ui->lineEdit_2->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_2->setText(strA);

    strB = ui->lineEdit_4->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_4->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_4()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_4->text();
    ui->lineEdit_27->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_4->setText("");
            ui->lineEdit_27->setText("");
        }
        return;
    }

    strB = ui->lineEdit_3->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_3->setText(strA);

    strB = ui->lineEdit_5->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_5->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_5()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_5->text();
    ui->lineEdit_26->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_5->setText("");
            ui->lineEdit_26->setText("");
        }
        return;
    }

    strB = ui->lineEdit_4->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_4->setText(strA);

    strB = ui->lineEdit_6->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_6->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_6()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_6->text();
    ui->lineEdit_25->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_6->setText("");
            ui->lineEdit_25->setText("");
        }
        return;
    }

    strB = ui->lineEdit_5->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_5->setText(strA);

    strB = ui->lineEdit_7->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_7->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_7()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_7->text();
    ui->lineEdit_24->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_7->setText("");
            ui->lineEdit_24->setText("");
        }
        return;
    }

    strB = ui->lineEdit_6->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_6->setText(strA);

    strB = ui->lineEdit_8->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_8->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_8()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_8->text();
    ui->lineEdit_23->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_8->setText("");
            ui->lineEdit_23->setText("");
        }
        return;
    }

    strB = ui->lineEdit_7->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_7->setText(strA);

    strB = ui->lineEdit_9->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_9->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_9()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_9->text();
    ui->lineEdit_22->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_9->setText("");
            ui->lineEdit_22->setText("");
        }
        return;
    }

    strB = ui->lineEdit_8->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_8->setText(strA);

    strB = ui->lineEdit_10->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_10->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_10()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_10->text();
    ui->lineEdit_21->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_10->setText("");
            ui->lineEdit_21->setText("");
        }
        return;
    }

    strB = ui->lineEdit_9->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_9->setText(strA);

    strB = ui->lineEdit_11->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_11->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_11()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_11->text();
    ui->lineEdit_20->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_11->setText("");
            ui->lineEdit_20->setText("");
        }
        return;
    }

    strB = ui->lineEdit_10->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_10->setText(strA);

    strB = ui->lineEdit_12->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_12->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_12()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_12->text();
    ui->lineEdit_19->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_12->setText("");
            ui->lineEdit_19->setText("");
        }
        return;
    }

    strB = ui->lineEdit_11->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_11->setText(strA);

    strB = ui->lineEdit_13->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_13->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_13()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_13->text();
    ui->lineEdit_18->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_13->setText("");
            ui->lineEdit_18->setText("");
        }
        return;
    }

    strB = ui->lineEdit_12->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_12->setText(strA);

    strB = ui->lineEdit_14->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_14->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_14()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_14->text();
    ui->lineEdit_17->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_14->setText("");
            ui->lineEdit_17->setText("");
        }
        return;
    }

    strB = ui->lineEdit_13->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_13->setText(strA);

    strB = ui->lineEdit_15->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_15->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditTar_15()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_15->text();
    ui->lineEdit_16->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( !b){
        if(strA.length()>= 0){
            ui->lineEdit_15->setText("");
            ui->lineEdit_16->setText("");
        }
        return;
    }

    strB = ui->lineEdit_14->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_14->setText(strA);
}


/****************************************************************************************/


void ProbeSettingSlctvtAdvIPage::onEditNonTar_1()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n1->text();
    ui->lineEdit_n30->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n1->setText("");
            ui->lineEdit_n30->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n2->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n2->setText(strA);

}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_2()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n2->text();
    ui->lineEdit_n29->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n2->setText("");
            ui->lineEdit_n29->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n1->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n1->setText(strA);

    strB = ui->lineEdit_n3->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n3->setText(strA);
}

void ProbeSettingSlctvtAdvIPage::onEditNonTar_3()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n3->text();
    ui->lineEdit_n28->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n3->setText("");
            ui->lineEdit_n28->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n2->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n2->setText(strA);

    strB = ui->lineEdit_n4->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n4->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_4()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n4->text();
    ui->lineEdit_n27->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n4->setText("");
            ui->lineEdit_n27->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n3->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n3->setText(strA);

    strB = ui->lineEdit_n5->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n5->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_5()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n5->text();
    ui->lineEdit_n26->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n5->setText("");
            ui->lineEdit_n26->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n4->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n4->setText(strA);

    strB = ui->lineEdit_n6->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n6->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_6()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n6->text();
    ui->lineEdit_n25->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n6->setText("");
            ui->lineEdit_n25->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n5->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n5->setText(strA);

    strB = ui->lineEdit_n7->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n7->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_7()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n7->text();
    ui->lineEdit_n24->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n7->setText("");
            ui->lineEdit_n24->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n6->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n6->setText(strA);

    strB = ui->lineEdit_n8->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n8->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_8()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n8->text();
    ui->lineEdit_n23->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n8->setText("");
            ui->lineEdit_n23->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n7->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n7->setText(strA);

    strB = ui->lineEdit_n9->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n9->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_9()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n9->text();
    ui->lineEdit_n22->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n9->setText("");
            ui->lineEdit_n22->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n8->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n8->setText(strA);

    strB = ui->lineEdit_n10->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n10->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_10()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n10->text();
    ui->lineEdit_n21->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n10->setText("");
            ui->lineEdit_n21->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n9->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n9->setText(strA);

    strB = ui->lineEdit_n11->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n11->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_11()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n11->text();
    ui->lineEdit_n20->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n11->setText("");
            ui->lineEdit_n20->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n10->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n10->setText(strA);

    strB = ui->lineEdit_n12->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n12->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_12()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n12->text();
    ui->lineEdit_n19->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n12->setText("");
            ui->lineEdit_n19->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n11->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n11->setText(strA);

    strB = ui->lineEdit_n13->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n13->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_13()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n13->text();
    ui->lineEdit_n18->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n13->setText("");
            ui->lineEdit_n18->setText("");
        }
        return;
    }
    strB = ui->lineEdit_n12->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n12->setText(strA);

    strB = ui->lineEdit_n14->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n14->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_14()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n14->text();
    ui->lineEdit_n17->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( b == false){
        if(strA.length()>= 0){
            ui->lineEdit_n14->setText("");
            ui->lineEdit_n17->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n13->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n13->setText(strA);

    strB = ui->lineEdit_n15->text();
    B = strB.toInt(&b);
    if(b && A < B )
        ui->lineEdit_n15->setText(strA);
}
void ProbeSettingSlctvtAdvIPage::onEditNonTar_15()
{
    bool b;
    QString strA, strB;
    strA = ui->lineEdit_n15->text();
    ui->lineEdit_n16->setText(strA);
    int A = strA.toInt(&b);
    int B;
    if( !b ){
        if(strA.length()>= 0){
            ui->lineEdit_n15->setText("");
            ui->lineEdit_n16->setText("");
        }
        return;
    }

    strB = ui->lineEdit_n14->text();
    B = strB.toInt(&b);
    if(b && A > B )
        ui->lineEdit_n14->setText(strA);

}


/******************************************************************************************/
