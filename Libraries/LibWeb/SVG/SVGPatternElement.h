/*
 * Copyright (c) 2025, Ankit Khandelwal <ankk98@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGfx/AffineTransform.h>
#include <LibWeb/DOM/Element.h>
#include <LibWeb/Painting/PaintStyle.h>
#include <LibWeb/SVG/AttributeParser.h>
#include <LibWeb/SVG/SVGAnimatedEnumeration.h>
#include <LibWeb/SVG/SVGAnimatedLength.h>
#include <LibWeb/SVG/SVGAnimatedTransformList.h>
#include <LibWeb/SVG/SVGElement.h>
#include <LibWeb/SVG/SVGURIReference.h>
#include <LibWeb/SVG/SVGViewport.h>

namespace Web::SVG {

class SVGPatternElement final : public SVGElement
    , public SVGURIReferenceMixin<SupportsXLinkHref::Yes>
    , public SVGViewport {
    WEB_PLATFORM_OBJECT(SVGPatternElement, SVGElement);

public:
    virtual ~SVGPatternElement() override = default;

    SVGPatternElement(DOM::Document&, DOM::QualifiedName);

    virtual void attribute_changed(FlyString const& name, Optional<String> const& old_value, Optional<String> const& value, Optional<FlyString> const& namespace_) override;
    virtual void initialize(JS::Realm&) override;
    virtual void visit_edges(Cell::Visitor&) override;

    // SVGFitToViewBox mixin support via SVGViewport
    virtual Optional<ViewBox> view_box() const override { return m_view_box; }
    virtual Optional<PreserveAspectRatio> preserve_aspect_ratio() const override { return {}; }

    // ImplementedAs=view_box_for_bindings for the mixin
    GC::Ref<SVGAnimatedRect> view_box_for_bindings() { return *m_view_box_for_bindings; }

    // Animated properties for IDL
    GC::Ref<SVGAnimatedLength> x();
    GC::Ref<SVGAnimatedLength> y();
    GC::Ref<SVGAnimatedLength> width();
    GC::Ref<SVGAnimatedLength> height();
    GC::Ref<SVGAnimatedEnumeration> pattern_units_for_bindings();
    GC::Ref<SVGAnimatedEnumeration> pattern_content_units_for_bindings();
    GC::Ref<SVGAnimatedTransformList> pattern_transform_for_bindings();

    // Internal resolved values used by painting
    Optional<float> x_value() const { return m_x_value; }
    Optional<float> y_value() const { return m_y_value; }
    Optional<float> width_value() const { return m_width_value; }
    Optional<float> height_value() const { return m_height_value; }
    Optional<NumberPercentage> x_number_percentage() const { return m_x_number_percentage; }
    Optional<NumberPercentage> y_number_percentage() const { return m_y_number_percentage; }
    Optional<NumberPercentage> width_number_percentage() const { return m_width_number_percentage; }
    Optional<NumberPercentage> height_number_percentage() const { return m_height_number_percentage; }
    Optional<SVGUnits> pattern_units_value() const { return m_pattern_units_value; }
    Optional<SVGUnits> pattern_content_units_value() const { return m_pattern_content_units_value; }
    Optional<Gfx::AffineTransform> pattern_transform_value() const { return m_pattern_transform_value; }

    // Inheritance via href/xlink:href
    GC::Ptr<SVGPatternElement const> linked_pattern(HashTable<SVGPatternElement const*>& seen_patterns) const;

private:
    Optional<float> m_x_value;
    Optional<float> m_y_value;
    Optional<float> m_width_value;
    Optional<float> m_height_value;
    Optional<SVGUnits> m_pattern_units_value;
    Optional<SVGUnits> m_pattern_content_units_value;
    Optional<Gfx::AffineTransform> m_pattern_transform_value;

    // Bindings objects
    GC::Ptr<SVGAnimatedLength> m_x;
    GC::Ptr<SVGAnimatedLength> m_y;
    GC::Ptr<SVGAnimatedLength> m_width;
    GC::Ptr<SVGAnimatedLength> m_height;
    GC::Ptr<SVGAnimatedEnumeration> m_pattern_units;
    GC::Ptr<SVGAnimatedEnumeration> m_pattern_content_units;
    GC::Ptr<SVGAnimatedTransformList> m_pattern_transform;

    // Raw numeric-or-percentage values for ObjectBoundingBox resolution
    Optional<NumberPercentage> m_x_number_percentage;
    Optional<NumberPercentage> m_y_number_percentage;
    Optional<NumberPercentage> m_width_number_percentage;
    Optional<NumberPercentage> m_height_number_percentage;

    Optional<ViewBox> m_view_box;
    GC::Ptr<SVGAnimatedRect> m_view_box_for_bindings;
};

}
