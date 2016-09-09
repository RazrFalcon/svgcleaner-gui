#include <QFile>

#include "process.h"
#include "compressor.h"

namespace CompressorName {
    const QString SevenZip = "7za";
    const QString Zopfli = "zopfli";
}

Compressor Compressor::fromName(const QString &aname)
{
    if (aname == Compressor(SevenZip).name()) {
        return Compressor(SevenZip);
    } else if (aname == Compressor(Zopfli).name()) {
        return Compressor(Zopfli);
    } else {
        Q_UNREACHABLE();
    }
}

QString Compressor::levelToString(int v) const
{
    Q_ASSERT(v >= 0 && v <= 4);

    if (m_type == SevenZip) {
        switch (v) {
            case 0 : return "-mx1";
            case 1 : return "-mx3";
            case 2 : return "-mx5";
            case 3 : return "-mx7";
            case 4 : return "-mx9";
        default: break;
        }
    } else {
        switch (v) {
            case 0 : return "--i1";
            case 1 : return "--i15";
            case 2 : return "--i50";
            case 3 : return "--i100";
            case 4 : return "--i500";
        default: break;
        }
    }

    Q_UNREACHABLE();
    return QString();
}

QString Compressor::name() const
{
    switch (m_type) {
        case None : Q_UNREACHABLE();
        case SevenZip : return CompressorName::SevenZip;
        case Zopfli : return CompressorName::Zopfli;
    }
    Q_UNREACHABLE();
}

static bool writeFile(const QString &path, const QByteArray &data)
{
    QFile file(path);
    if (file.open(QFile::WriteOnly)) {
        const qint64 size = file.write(data);
        if (size != data.size()) {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

// only 7za can unzip files
bool Compressor::unzip(const QString &inFile, const QString &outFile)
{
    auto res = Process::run(Compressor(SevenZip).name(), { "e", "-so", inFile });
    if (!res) {
        return false;
    }

    return writeFile(outFile, *res);
}

bool Compressor::zip(int lvl, const QString &inFile, const QString &outFile) const
{
    // remove previously created svgz file
    QFile(outFile).remove();

    const QString lvlStr = levelToString(lvl);
    if (m_type == SevenZip) {
        auto res = Process::run(name(), { "a", "-tgzip", "-y", lvlStr,
                                          outFile, inFile });
        if (!res) {
            return false;
        }
    } else if (m_type == Zopfli) {
        // TODO: generate stat to find max --i value that is actually make sense

        auto res = Process::run(name(), { "-c", lvlStr, inFile }, 300000); // 5min
        if (!res) {
            return false;
        }

        if (!writeFile(outFile, *res)) {
            return false;
        }
    } else {
        Q_UNREACHABLE();
    }

    // TODO: if we have two file with the same name, but with different extensions
    //       and compression is enabled - we will get one file, instead of two
    // remove svg file
    QFile(inFile).remove();

    return true;
}
