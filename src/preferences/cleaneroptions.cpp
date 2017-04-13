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
        const QString GroupByStyle              = "group-by-style";
        const QString MergeGradients            = "merge-gradients";
        const QString RegroupGradientStops      = "regroup-gradient-stops";
        const QString RemoveInvalidStops        = "remove-invalid-stops";
        const QString RemoveInvisibleElements   = "remove-invisible-elements";
        const QString ResolveUse                = "resolve-use";
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
        const QString JoinStyleAttributes       = "join-style-attributes";
        const QString ApplyTransformToGradients = "apply-transform-to-gradients";
        const QString ApplyTransformToShapes    = "apply-transform-to-shapes";
        const QString RemoveUnresolvedClasses   = "remove-unresolved-classes";
    }

    namespace Paths {
        const QString PathsToRelative           = "paths-to-relative";
        const QString RemoveUnusedSegments      = "remove-unused-segments";
        const QString ConvertSegments           = "convert-segments";
        const QString ApplyTransformToPaths     = "apply-transform-to-paths";
        const QString TrimPaths                 = "trim-paths";
        const QString RemoveDuplCmdInPaths      = "remove-dupl-cmd-in-paths";
        const QString JoinArcToFlags            = "join-arcto-flags";
        const QString UseImplicitCommands       = "use-implicit-cmds";
    }

    namespace Output {
        const QString TrimColors                = "trim-colors";
        const QString SimplifyTransforms        = "simplify-transforms";
        const QString PathsPrecision            = "paths-coordinates-precision";
        const QString Indent                    = "indent";
    }

    namespace Other {
        const QString Multipass                 = "multipass";
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
        hash.insert(Elements::GroupByStyle, true);
        hash.insert(Elements::MergeGradients, true);
        hash.insert(Elements::RegroupGradientStops, true);
        hash.insert(Elements::RemoveInvalidStops, true);
        hash.insert(Elements::RemoveInvisibleElements, true);
        hash.insert(Elements::ResolveUse, true);

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
        hash.insert(Attributes::JoinStyleAttributes, "some");
        hash.insert(Attributes::ApplyTransformToGradients, true);
        hash.insert(Attributes::ApplyTransformToShapes, true);
        hash.insert(Attributes::RemoveUnresolvedClasses, true);

        hash.insert(Paths::PathsToRelative, true);
        hash.insert(Paths::RemoveUnusedSegments, true);
        hash.insert(Paths::ConvertSegments, true);
        hash.insert(Paths::ApplyTransformToPaths, false);
        hash.insert(Paths::TrimPaths, true);
        hash.insert(Paths::RemoveDuplCmdInPaths, true);
        hash.insert(Paths::JoinArcToFlags, false);
        hash.insert(Paths::UseImplicitCommands, true);

        hash.insert(Output::TrimColors, true);
        hash.insert(Output::SimplifyTransforms, true);
        hash.insert(Output::PathsPrecision, 8);
        hash.insert(Output::Indent, "none");

        hash.insert(Other::Multipass, false);
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

QString CleanerOptions::defaultString(const QString &key)
{
    return defaultValue(key).toString();
}

static void genValue(const QString &key, const QString &value, QStringList &list)
{
    list << ("--" + key + "=" + value);
}

static void genFlag(const QString &key, QStringList &list)
{
    const bool f = CleanerOptions().flag(key);
    if (CleanerOptions::defaultFlag(key) != f) {
        genValue(key, f ? "true" : "false", list);
    }
}

static void genNumFlag(const QString &key, QStringList &list)
{
    const int v = CleanerOptions().integer(key);
    if (CleanerOptions::defaultInt(key) != v) {
        genValue(key, QString::number(v), list);
    }
}

static void genStringFlag(const QString &key, QStringList &list)
{
    const QString v = CleanerOptions().string(key);
    if (CleanerOptions::defaultString(key) != v) {
        genValue(key, v, list);
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
        Elements::GroupByStyle,
        Elements::MergeGradients,
        Elements::RegroupGradientStops,
        Elements::RemoveInvalidStops,
        Elements::RemoveInvisibleElements,
        Elements::ResolveUse,
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
        Attributes::ApplyTransformToGradients,
        Attributes::ApplyTransformToShapes,
        Attributes::RemoveUnresolvedClasses,
    };
    for (const QString &name : attrList) {
        genFlag(name, list);
    }
    genStringFlag(Attributes::JoinStyleAttributes, list);

    const auto pathList = {
        Paths::PathsToRelative,
        Paths::RemoveUnusedSegments,
        Paths::ConvertSegments,
        Paths::ApplyTransformToPaths,
        Paths::TrimPaths,
        Paths::RemoveDuplCmdInPaths,
        Paths::JoinArcToFlags,
        Paths::UseImplicitCommands,
    };
    for (const QString &name : pathList) {
        genFlag(name, list);
    }

    genFlag(Output::TrimColors, list);
    genFlag(Output::SimplifyTransforms, list);
    genNumFlag(Output::PathsPrecision, list);
    genStringFlag(Output::Indent, list);

    genFlag(Other::Multipass, list);

    return list;
}
