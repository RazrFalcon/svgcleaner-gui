#pragma once

#include <QString>

namespace CompressorName {
    extern const QString SevenZip;
    extern const QString Zopfli;
}

class Compressor
{
public:
    enum Type { None, SevenZip, Zopfli };

    Compressor(Type t) : m_type(t) {}
    static Compressor fromName(const QString &aname);

    QString levelToString(int v) const;
    QString name() const;
    Type type() const
    { return m_type; }

    bool zip(int lvl, const QString &inFile, const QString &outFile) const;
    static bool unzip(const QString &inFile, const QString &outFile);

private:
    Type m_type = None;
};

