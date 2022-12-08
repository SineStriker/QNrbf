#include "ScoreMarkUtils.h"

#include "Utils/ListUtils.h"

using Tempo = QSvipModel::SongTempo;
using TimeSig = QSvipModel::TimeSignature;

QList<Tempo> ScoreMarkUtils::SkipTempoList(const QList<Tempo> &tempoList, int skipTicks) {
    auto result = ListUtils::Select(
        ListUtils::Where(tempoList,
                         [&](const Tempo &tempo) { return tempo.Position >= skipTicks; }),
        [&](const Tempo &tempo) {
            return Tempo{tempo.Position - skipTicks, tempo.BPM};
        });

    if (!result.isEmpty() && result.front().Position <= 0) {
        return result;
    }

    int i = 0;
    for (; i < tempoList.size() && tempoList[i].Position <= skipTicks; i++) {
    }

    result.insert(0, Tempo{0, tempoList[i - 1].BPM});
    return result;
}

QList<TimeSig> ScoreMarkUtils::SkipBeatList(const QList<TimeSig> &beatList, int skipBars) {
    auto result = ListUtils::Select(
        ListUtils::Where(beatList, [&](const TimeSig &beat) { return beat.BarIndex >= skipBars; }),
        [&](const TimeSig &meter) {
            return TimeSig{meter.BarIndex - skipBars, meter.Numerator, meter.Denominator};
        });

    if (result.isEmpty() || result.front().BarIndex > 0) {
        result.insert(0, TimeSig{0, beatList.front().Numerator, beatList.front().Denominator});
    }
    return result;
}

QList<Tempo> ScoreMarkUtils::ShiftTempoList(const QList<Tempo> &tempoList, int shiftTicks) {
    QList<Tempo> result = {tempoList.front()};
    auto tmp = ListUtils::Select(tempoList.mid(1), [&](const Tempo &tempo) {
        return Tempo{tempo.Position + shiftTicks, tempo.BPM};
    });
    result.append(tmp);
    return result;
}

QList<TimeSig> ScoreMarkUtils::ShiftBeatList(const QList<TimeSig> &beatList, int shiftBars) {
    QList<TimeSig> result = {beatList.front()};
    auto tmp = ListUtils::Select(beatList.mid(1), [&](const TimeSig &beat) {
        return TimeSig{beat.BarIndex + shiftBars, beat.Numerator, beat.Denominator};
    });
    result.append(tmp);
    return result;
}