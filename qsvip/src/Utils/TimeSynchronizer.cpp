#include "TimeSynchronizer.h"

#include "ScoreMarkUtils.h"

#include <algorithm>

TimeSynchronizer::TimeSynchronizer(const QList<QSvipModel::SongTempo> &originalTempoList,
                                   int skipTicks, bool isAbsoluteTimeMode, int defaultTempo)
    : defaultTempo(defaultTempo),
      m_tempoList(skipTicks > 0 ? ScoreMarkUtils::SkipTempoList(originalTempoList, skipTicks)
                                : originalTempoList),
      m_isAbsoluteTimeMode(isAbsoluteTimeMode) {
}

TimeSynchronizer::~TimeSynchronizer() = default;

double TimeSynchronizer::actualTicksFromTicks(int ticks) const {
    if (!m_isAbsoluteTimeMode) {
        return ticks;
    }
    double res = 0.0;
    int i = 0;
    for (; i < m_tempoList.size() - 1 && m_tempoList[i + 1].Position < ticks; i++) {
        res += (m_tempoList[i + 1].Position - m_tempoList[i].Position) * defaultTempo /
               m_tempoList[i].BPM;
    }
    res += (ticks - m_tempoList[i].Position) * defaultTempo / m_tempoList[i].BPM;
    return res;
}

double TimeSynchronizer::actualSecsFromTicks(int ticks) const {
    return durationSecsFromTicks(0, ticks);
}

double TimeSynchronizer::actualTicksFromSecs(double secs) const {
    return actualTicksFromSecsOffset(0, secs);
}

double TimeSynchronizer::durationSecsFromTicks(int startTicks, int endTicks) const {
    if (m_isAbsoluteTimeMode) {
        return (actualTicksFromTicks(endTicks) - actualTicksFromTicks(startTicks)) / defaultTempo /
               8;
    }

    int startTempoIndex = findLastIndexLE(m_tempoList, startTicks);
    int endTempoIndex = findLastIndexLE(m_tempoList, endTicks);
    if (startTempoIndex == endTempoIndex) {
        return (endTicks - startTicks) / m_tempoList[startTempoIndex].BPM / 8;
    }

    double secs = 0.0;
    secs += (m_tempoList[startTempoIndex + 1].Position - startTicks) /
            double(m_tempoList[startTempoIndex].BPM) / 8;
    for (int i = startTempoIndex + 1; i < endTempoIndex; i++) {
        secs += (m_tempoList[i + 1].Position - m_tempoList[i].Position) /
                double(m_tempoList[i].BPM) / 8;
    }
    secs += (endTicks - m_tempoList[endTempoIndex].Position) /
            double(m_tempoList[endTempoIndex].BPM) / 8;
    return secs;
}

double TimeSynchronizer::actualTicksFromSecsOffset(int startTicks, double offsetSecs) const {
    if (m_isAbsoluteTimeMode) {
        return actualTicksFromTicks(startTicks) + defaultTempo * 8 * offsetSecs;
    }

    int startTempoIndex = findLastIndexLE(m_tempoList, startTicks);
    double ticks = startTicks;
    double secs = offsetSecs;
    for (int i = startTempoIndex; i < m_tempoList.size() - 1; i++) {
        auto dur = (m_tempoList[i + 1].Position - ticks) / m_tempoList[i].BPM / 8;
        if (dur < secs) {
            ticks = m_tempoList[i + 1].Position;
            secs -= dur;
        } else {
            ticks += (m_tempoList[i + 1].Position - ticks) * secs / dur;
            return ticks;
        }
    }
    ticks += m_tempoList.last().BPM * 8 * secs;
    return ticks;
}

int TimeSynchronizer::findLastIndexLE(const QList<QSvipModel::SongTempo> &tempoList, int tick) {
    // Comparator
    struct SongTempoGreater {
        bool operator()(const QSvipModel::SongTempo &left,
                        const QSvipModel::SongTempo &right) const {
            return left.Position > right.Position;
        }
    };

    // Binary search
    auto it = std::lower_bound(tempoList.rbegin(), tempoList.rend(),
                               QSvipModel::SongTempo{tick, 120.0}, SongTempoGreater());

    // Get index
    return tempoList.size() - (it - tempoList.rbegin()) - 1; // If not found then return -1
}