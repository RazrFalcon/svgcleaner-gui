/****************************************************************************
**
** SVG Cleaner could help you to clean up your SVG files
** from unnecessary data.
** Copyright (C) 2012-2016 Evgeniy Reizner
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
****************************************************************************/

#include "cleaneroptions.h"

namespace CleanerKey {
    namespace Elements {
        const QString RemoveComments            = "remove-comments";
        const QString RemoveDeclaration         = "remove-declarations";
        const QString RemoveNonSvgElements      = "remove-nonsvg-elements";
        const QString RemoveUnusedDefs          = "remove-unused-defs";
        const QString ConvertBasicShapes        = "convert-shapes";
        const QString RemoveTitle               = "remove-title";
        const QString RemoveDesc                = "remove-desc";
        const QString RemoveMetadata            = "remove-metadata";
        const QString RemoveDuplLinearGradient  = "remove-dupl-lineargradient";
        const QString RemoveDuplRadialGradient  = "remove-dupl-radialgradient";
        const QString UngroupGroups             = "ungroup-groups";
        const QString MergeGradients            = "merge-gradients";
        const QString RemoveInvisibleElements   = "remove-invisible-elements";
    }

    namespace Attributes {
        const QString RemoveVersion             = "remove-version";
        const QString RemoveNonSvgAttributes    = "remove-nonsvg-attributes";
        const QString RemoveUnrefIds            = "remove-unreferenced-ids";
        const QString TrimIds                   = "trim-ids";
        const QString RemoveTextAttributes      = "remove-text-attributes";
        const QString RemoveUnusedCoordinates   = "remove-unused-coordinates";
        const QString RemoveDefaultAttributes   = "remove-default-attributes";
        const QString RemoveXmlnsXlinkAttribute = "remove-xmlns-xlink-attribute";
        const QString RemoveNeedlessAttributes  = "remove-needless-attributes";
        const QString MoveStylesToGroup         = "move-styles-to-group";
        const QString JoinStyleAttributes       = "join-style-attributes";
    }

    namespace Paths {
        const QString PathsToRelative           = "paths-to-relative";
        const QString RemoveUnusedSegments      = "remove-unused-segments";
        const QString TrimPaths                 = "trim-paths";
        const QString RemoveDuplCmdInPaths      = "remove-dupl-cmd-in-paths";
        const QString JoinArcToFlags            = "join-arcto-flags";
    }

    namespace Output {
        const QString PrecisionCoordinate       = "precision-coordinate";
        const QString PrecisionTransform        = "precision-transform";
        const QString TrimColors                = "trim-colors";
        const QString SimplifyTransforms        = "simplify-transforms";
        const QString Indent                    = "indent";
    }
}

CleanerOptions::CleanerOptions(QObject *parent) :
    QSettings(QSettings::NativeFormat, QSettings::UserScope,
              "SVGCleaner", "svgcleaner-options", parent)
{}

bool CleanerOptions::flag(const QString &key)
{
    return value(key, defaultValue(key)).toBool();
}

int CleanerOptions::integer(const QString &key)
{
    return value(key, defaultValue(key)).toInt();
}

QString CleanerOptions::string(const QString &key)
{
    return value(key, defaultValue(key)).toString();
}

QVariant CleanerOptions::defaultValue(const QString &key)
{
    using namespace CleanerKey;

    static QHash<QString, QVariant> hash;
    if (hash.isEmpty()) {
        hash.insert(Elements::RemoveComments, true);
        hash.insert(Elements::RemoveDeclaration, true);
        hash.insert(Elements::RemoveNonSvgElements, true);
        hash.insert(Elements::RemoveUnusedDefs, true);
        hash.insert(Elements::ConvertBasicShapes, true);
        hash.insert(Elements::RemoveTitle, true);
        hash.insert(Elements::RemoveDesc, true);
        hash.insert(Elements::RemoveMetadata, true);
        hash.insert(Elements::RemoveDuplLinearGradient, true);
        hash.insert(Elements::RemoveDuplRadialGradient, true);
        hash.insert(Elements::UngroupGroups, true);
        hash.insert(Elements::MergeGradients, true);
        hash.insert(Elements::RemoveInvisibleElements, true);

        hash.insert(Attributes::RemoveVersion, true);
        hash.insert(Attributes::RemoveNonSvgAttributes, true);
        hash.insert(Attributes::RemoveUnrefIds, true);
        hash.insert(Attributes::TrimIds, true);
        hash.insert(Attributes::RemoveTextAttributes, true);
        hash.insert(Attributes::RemoveUnusedCoordinates, true);
        hash.insert(Attributes::RemoveDefaultAttributes, true);
        hash.insert(Attributes::RemoveXmlnsXlinkAttribute, true);
        hash.insert(Attributes::RemoveNeedlessAttributes, true);
        hash.insert(Attributes::MoveStylesToGroup, true);
        hash.insert(Attributes::JoinStyleAttributes, true);

        hash.insert(Paths::PathsToRelative, true);
        hash.insert(Paths::RemoveUnusedSegments, true);
        hash.insert(Paths::TrimPaths, true);
        hash.insert(Paths::RemoveDuplCmdInPaths, true);
        hash.insert(Paths::JoinArcToFlags, false);

        hash.insert(Output::PrecisionCoordinate, 6);
        hash.insert(Output::PrecisionTransform, 8);
        hash.insert(Output::TrimColors, true);
        hash.insert(Output::SimplifyTransforms, true);
        hash.insert(Output::Indent, -1);
    }

    Q_ASSERT(hash.contains(key) == true);

    return hash.value(key);
}

bool CleanerOptions::defaultFlag(const QString &key)
{
    return defaultValue(key).toBool();
}

int CleanerOptions::defaultInt(const QString &key)
{
    return defaultValue(key).toInt();
}

static void genFlag(const QString &key, QStringList &list)
{
    if (CleanerOptions::defaultFlag(key) != CleanerOptions().flag(key)) {
        list << ("--" + key + "=" + (CleanerOptions().flag(key) ? "true" : "false"));
    }
}

static void genNumFlag(const QString &key, QStringList &list)
{
    if (CleanerOptions::defaultInt(key) != CleanerOptions().integer(key)) {
        list << ("--" + key + "=" + QString::number(CleanerOptions().integer(key)));
    }
}

QStringList CleanerOptions::genArgs()
{
    using namespace CleanerKey;

    QStringList list;
    genFlag(Elements::RemoveComments, list);
    genFlag(Elements::RemoveDeclaration, list);
    genFlag(Elements::RemoveNonSvgElements, list);
    genFlag(Elements::RemoveUnusedDefs, list);
    genFlag(Elements::ConvertBasicShapes, list);
    genFlag(Elements::RemoveTitle, list);
    genFlag(Elements::RemoveDesc, list);
    genFlag(Elements::RemoveMetadata, list);
    genFlag(Elements::RemoveDuplLinearGradient, list);
    genFlag(Elements::RemoveDuplRadialGradient, list);
    genFlag(Elements::UngroupGroups, list);
    genFlag(Elements::MergeGradients, list);
    genFlag(Elements::RemoveInvisibleElements, list);

    genFlag(Attributes::RemoveVersion, list);
    genFlag(Attributes::RemoveNonSvgAttributes, list);
    genFlag(Attributes::RemoveUnrefIds, list);
    genFlag(Attributes::TrimIds, list);
    genFlag(Attributes::RemoveTextAttributes, list);
    genFlag(Attributes::RemoveUnusedCoordinates, list);
    genFlag(Attributes::RemoveDefaultAttributes, list);
    genFlag(Attributes::RemoveXmlnsXlinkAttribute, list);
    genFlag(Attributes::RemoveNeedlessAttributes, list);
    genFlag(Attributes::MoveStylesToGroup, list);
    genFlag(Attributes::JoinStyleAttributes, list);

    genFlag(Paths::PathsToRelative, list);
    genFlag(Paths::RemoveUnusedSegments, list);
    genFlag(Paths::TrimPaths, list);
    genFlag(Paths::RemoveDuplCmdInPaths, list);
    genFlag(Paths::JoinArcToFlags, list);

    genNumFlag(Output::PrecisionCoordinate, list);
    genNumFlag(Output::PrecisionTransform, list);
    genFlag(Output::TrimColors, list);
    genFlag(Output::SimplifyTransforms, list);
    genNumFlag(Output::Indent, list);

    return list;
}
