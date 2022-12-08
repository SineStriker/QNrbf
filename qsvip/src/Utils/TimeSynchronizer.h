#ifndef QNRBFFORMAT_TIMESYNCHRONIZER_H
#define QNRBFFORMAT_TIMESYNCHRONIZER_H

#include <QStringList>

#include "QSvipModel.h"

class TimeSynchronizer {
public:
    explicit TimeSynchronizer(const QList<QSvipModel::SongTempo> &originalTempoList,
                              int skipTicks = 0, bool isAbsoluteTimeMode = false,
                              int defaultTempo = 60);
    ~TimeSynchronizer();

    double defaultTempo;

    // 将原始谱面位置（梯）转换为对齐后的实际谱面位置（梯）。
    double actualTicksFromTicks(int ticks) const;

    // 将原始谱面位置（梯）转换为对齐后的实际谱面时间坐标（秒）。
    double actualSecsFromTicks(int ticks) const;

    // 将原始谱面时间坐标（秒）转换为对齐后的实际谱面位置（梯）。
    double actualTicksFromSecs(double secs) const;

    // 计算原始谱面上两个位置坐标（梯）在对齐后的时间差（秒）。
    double durationSecsFromTicks(int startTicks, int endTicks) const;

    // 计算原始谱面上从某位置（梯）开始经过给定时间（秒）后所对应的实际位置（梯）。
    double actualTicksFromSecsOffset(int startTicks, double offsetSecs) const;

protected:
    QList<QSvipModel::SongTempo> m_tempoList;
    const bool m_isAbsoluteTimeMode;

public:
    // 返回曲速列表中最后一个位置小于等于tick的下标，如果没有则返回-1
    static int findLastIndexLE(const QList<QSvipModel::SongTempo> &tempoList, int tick);
};


#endif // QNRBFFORMAT_TIMESYNCHRONIZER_H
