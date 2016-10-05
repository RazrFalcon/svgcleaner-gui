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
        const QString RemoveDuplFeGaussianBlur  = "remove-dupl-fegaussianblur";
        const QString UngroupGroups             = "ungroup-groups";
        const QString UngroupDefs               = "ungroup-defs";
        const QString MergeGradients            = "merge-gradients";
        const QString RegroupGradientStops      = "regroup-gradient-stops";
        const QString RemoveInvalidStops        = "remove-invalid-stops";
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
        const QString RemoveGradientAttributes  = "remove-gradient-attributes";
        const QString MoveStylesToGroup         = "move-styles-to-group";
        const QString JoinStyleAttributes       = "join-style-attributes";
        const QString ApplyTransformToGradients = "apply-transform-to-gradients";
    }

    namespace Paths {
        const QString PathsToRelative           = "paths-to-relative";
        const QString RemoveUnusedSegments      = "remove-unused-segments";
        const QString TrimPaths                 = "trim-paths";
        const QString RemoveDuplCmdInPaths      = "remove-dupl-cmd-in-paths";
        const QString JoinArcToFlags            = "join-arcto-flags";
    }

    namespace Output {
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
        hash.insert(Elements::RemoveDuplFeGaussianBlur, true);
        hash.insert(Elements::UngroupGroups, true);
        hash.insert(Elements::UngroupDefs, true);
        hash.insert(Elements::MergeGradients, true);
        hash.insert(Elements::RegroupGradientStops, true);
        hash.insert(Elements::RemoveInvalidStops, true);
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
        hash.insert(Attributes::RemoveGradientAttributes, false);
        hash.insert(Attributes::MoveStylesToGroup, true);
        hash.insert(Attributes::JoinStyleAttributes, true);
        hash.insert(Attributes::ApplyTransformToGradients, true);

        hash.insert(Paths::PathsToRelative, true);
        hash.insert(Paths::RemoveUnusedSegments, true);
        hash.insert(Paths::TrimPaths, true);
        hash.insert(Paths::RemoveDuplCmdInPaths, true);
        hash.insert(Paths::JoinArcToFlags, false);

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

    const auto elemList = {
        Elements::RemoveComments,
        Elements::RemoveDeclaration,
        Elements::RemoveNonSvgElements,
        Elements::RemoveUnusedDefs,
        Elements::ConvertBasicShapes,
        Elements::RemoveTitle,
        Elements::RemoveDesc,
        Elements::RemoveMetadata,
        Elements::RemoveDuplLinearGradient,
        Elements::RemoveDuplRadialGradient,
        Elements::RemoveDuplFeGaussianBlur,
        Elements::UngroupGroups,
        Elements::UngroupDefs,
        Elements::MergeGradients,
        Elements::RegroupGradientStops,
        Elements::RemoveInvalidStops,
        Elements::RemoveInvisibleElements,
    };
    for (const QString &name : elemList) {
        genFlag(name, list);
    }

    const auto attrList = {
        Attributes::RemoveVersion,
        Attributes::RemoveNonSvgAttributes,
        Attributes::RemoveUnrefIds,
        Attributes::TrimIds,
        Attributes::RemoveTextAttributes,
        Attributes::RemoveUnusedCoordinates,
        Attributes::RemoveDefaultAttributes,
        Attributes::RemoveXmlnsXlinkAttribute,
        Attributes::RemoveNeedlessAttributes,
        Attributes::RemoveGradientAttributes,
        Attributes::MoveStylesToGroup,
        Attributes::JoinStyleAttributes,
        Attributes::ApplyTransformToGradients,
    };
    for (const QString &name : attrList) {
        genFlag(name, list);
    }

    const auto pathList = {
        Paths::PathsToRelative,
        Paths::RemoveUnusedSegments,
        Paths::TrimPaths,
        Paths::RemoveDuplCmdInPaths,
        Paths::JoinArcToFlags,
    };
    for (const QString &name : pathList) {
        genFlag(name, list);
    }

    genFlag(Output::TrimColors, list);
    genFlag(Output::SimplifyTransforms, list);
    genNumFlag(Output::Indent, list);

    return list;
}
