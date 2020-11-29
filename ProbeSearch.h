#ifndef PROBESEARCH_H
#define PROBESEARCH_H
#include <QVector>
#include <QMessageBox>
#include "GlobalDef.h"
#include "Data.h"

class ProbeSearch
{
public:
    short probe[MAX_PROBE_LEN];
    int probeLen;
    QVector<int> bindingPosStartTarget;
    QVector<int> bindingPosEndTarget;
    QVector<int> bindingPosStartNonTar;
    QVector<int> bindingPosEndNonTar;

    ProbeSearch(short* pr);
    ~ProbeSearch(void);
    int searchProbeInTarSeqs(QVector<short*>);
    int searchProbeInNonTarSeqs(QVector<short*>);
    int appxSearchTarSeqs(short*, int&, int&);
    int appxSearchNonTarSeqs(short*, int&, int&);
};

#endif // PROBESEARCH_H
