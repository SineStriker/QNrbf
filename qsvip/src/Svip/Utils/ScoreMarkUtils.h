#ifndef QNRBFFORMAT_SCOREMARKUTILS_H
#define QNRBFFORMAT_SCOREMARKUTILS_H

#include "Svip/QSvipModel.h"

namespace ScoreMarkUtils {
    // 给定曲速列表从前端截去一定长度，并返回以截断处为零点的新曲速列表。
    QList<QSvipModel::SongTempo> SkipTempoList(const QList<QSvipModel::SongTempo> &tempoList,
                                               int skipTicks);

    // 截断给定拍号列表前面的若干个小节，并返回以截断处为零点的新拍号列表。
    QList<QSvipModel::TimeSignature> SkipBeatList(const QList<QSvipModel::TimeSignature> &beatList,
                                                  int skipBars);

    QList<QSvipModel::SongTempo> ShiftTempoList(const QList<QSvipModel::SongTempo> &tempoList,
                                                int shiftTicks);

    QList<QSvipModel::TimeSignature> ShiftBeatList(const QList<QSvipModel::TimeSignature> &beatList,
                                                   int shiftBars);
}; // namespace ScoreMarkUtils

#endif // QNRBFFORMAT_SCOREMARKUTILS_H
