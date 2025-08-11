/*
 * Copyright (c) 2025, Ankit Khandelwal <ankk98@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/Bindings/SVGPatternElementPrototype.h>
#include <LibWeb/SVG/AttributeNames.h>
#include <LibWeb/SVG/AttributeParser.h>
#include <LibWeb/SVG/SVGAnimatedRect.h>
#include <LibWeb/SVG/SVGGraphicsElement.h>
#include <LibWeb/SVG/SVGPatternElement.h>

namespace Web::SVG {

SVGPatternElement::SVGPatternElement(DOM::Document& document, DOM::QualifiedName qualified_name)
    : SVGElement(document, move(qualified_name))
{
}

void SVGPatternElement::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(SVGPatternElement);
    Base::initialize(realm);
    m_view_box_for_bindings = realm.create<SVGAnimatedRect>(realm);
}

void SVGPatternElement::attribute_changed(FlyString const& name, Optional<String> const& old_value, Optional<String> const& value, Optional<FlyString> const& namespace_)
{
    Base::attribute_changed(name, old_value, value, namespace_);

    auto const& v = value.value_or(String {});
    if (name == AttributeNames::x) {
        auto num_pct = AttributeParser::parse_number_percentage(v);
        m_x_number_percentage = num_pct;
        auto parsed = AttributeParser::parse_coordinate(v);
        m_x_value = parsed;
        if (!m_x)
            m_x = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
        MUST(m_x->base_val()->set_value(parsed.value_or(0)));
    } else if (name == AttributeNames::y) {
        auto num_pct = AttributeParser::parse_number_percentage(v);
        m_y_number_percentage = num_pct;
        auto parsed = AttributeParser::parse_coordinate(v);
        m_y_value = parsed;
        if (!m_y)
            m_y = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
        MUST(m_y->base_val()->set_value(parsed.value_or(0)));
    } else if (name == AttributeNames::width) {
        auto num_pct = AttributeParser::parse_number_percentage(v);
        m_width_number_percentage = num_pct;
        auto parsed = AttributeParser::parse_positive_length(v);
        m_width_value = parsed;
        if (!m_width)
            m_width = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
        MUST(m_width->base_val()->set_value(parsed.value_or(0)));
    } else if (name == AttributeNames::height) {
        auto num_pct = AttributeParser::parse_number_percentage(v);
        m_height_number_percentage = num_pct;
        auto parsed = AttributeParser::parse_positive_length(v);
        m_height_value = parsed;
        if (!m_height)
            m_height = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
        MUST(m_height->base_val()->set_value(parsed.value_or(0)));
    } else if (name == AttributeNames::patternUnits) {
        auto parsed = AttributeParser::parse_units(v);
        m_pattern_units_value = parsed;
        if (!m_pattern_units)
            m_pattern_units = SVGAnimatedEnumeration::create(realm(), 0);
        MUST(m_pattern_units->set_base_val(parsed.has_value() && parsed.value() == SVGUnits::UserSpaceOnUse ? 1 : 2));
    } else if (name == AttributeNames::patternContentUnits) {
        auto parsed = AttributeParser::parse_units(v);
        m_pattern_content_units_value = parsed;
        if (!m_pattern_content_units)
            m_pattern_content_units = SVGAnimatedEnumeration::create(realm(), 0);
        MUST(m_pattern_content_units->set_base_val(parsed.has_value() && parsed.value() == SVGUnits::UserSpaceOnUse ? 1 : 2));
    } else if (name == AttributeNames::patternTransform) {
        if (!m_pattern_transform)
            m_pattern_transform = SVGAnimatedTransformList::create(realm(), SVGTransformList::create(realm()), SVGTransformList::create(realm()));
        if (auto list = AttributeParser::parse_transform(v); list.has_value()) {
            m_pattern_transform_value = transform_from_transform_list(*list);
            // NOTE: We currently expose only the animated list object; full list mutation not implemented.
        } else {
            m_pattern_transform_value = {};
        }
    } else if (name.equals_ignoring_ascii_case(AttributeNames::viewBox)) {
        m_view_box = try_parse_view_box(v);
        m_view_box_for_bindings->set_nulled(!m_view_box.has_value());
        if (m_view_box.has_value()) {
            m_view_box_for_bindings->set_base_val(Gfx::DoubleRect { m_view_box->min_x, m_view_box->min_y, m_view_box->width, m_view_box->height });
            m_view_box_for_bindings->set_anim_val(Gfx::DoubleRect { m_view_box->min_x, m_view_box->min_y, m_view_box->width, m_view_box->height });
        }
    }
}

void SVGPatternElement::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    SVGURIReferenceMixin::visit_edges(visitor);
    visitor.visit(m_x);
    visitor.visit(m_y);
    visitor.visit(m_width);
    visitor.visit(m_height);
    visitor.visit(m_pattern_units);
    visitor.visit(m_pattern_content_units);
    visitor.visit(m_pattern_transform);
    visitor.visit(m_view_box_for_bindings);
}

GC::Ref<SVGAnimatedLength> SVGPatternElement::x()
{
    if (!m_x)
        m_x = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
    return *m_x;
}

GC::Ref<SVGAnimatedLength> SVGPatternElement::y()
{
    if (!m_y)
        m_y = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
    return *m_y;
}

GC::Ref<SVGAnimatedLength> SVGPatternElement::width()
{
    if (!m_width)
        m_width = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
    return *m_width;
}

GC::Ref<SVGAnimatedLength> SVGPatternElement::height()
{
    if (!m_height)
        m_height = SVGAnimatedLength::create(realm(), SVGLength::create(realm(), 0, 0), SVGLength::create(realm(), 0, 0));
    return *m_height;
}

GC::Ref<SVGAnimatedEnumeration> SVGPatternElement::pattern_units_for_bindings()
{
    if (!m_pattern_units)
        m_pattern_units = SVGAnimatedEnumeration::create(realm(), 0);
    return *m_pattern_units;
}

GC::Ref<SVGAnimatedEnumeration> SVGPatternElement::pattern_content_units_for_bindings()
{
    if (!m_pattern_content_units)
        m_pattern_content_units = SVGAnimatedEnumeration::create(realm(), 0);
    return *m_pattern_content_units;
}

GC::Ref<SVGAnimatedTransformList> SVGPatternElement::pattern_transform_for_bindings()
{
    if (!m_pattern_transform)
        m_pattern_transform = SVGAnimatedTransformList::create(realm(), SVGTransformList::create(realm()), SVGTransformList::create(realm()));
    return *m_pattern_transform;
}

GC::Ptr<SVGPatternElement const> SVGPatternElement::linked_pattern(HashTable<SVGPatternElement const*>& seen_patterns) const
{
    // Similar to SVGGradientElement::linked_gradient
    auto link = has_attribute(AttributeNames::href) ? get_attribute(AttributeNames::href) : get_attribute("xlink:href"_fly_string);
    if (auto href = link; href.has_value() && !link->is_empty()) {
        auto url = document().encoding_parse_url(*href);
        if (!url.has_value())
            return {};
        auto id = url->fragment();
        if (!id.has_value() || id->is_empty())
            return {};
        auto element = document().get_element_by_id(id.value());
        if (!element)
            return {};
        if (element == this)
            return {};
        if (!is<SVGPatternElement>(*element))
            return {};
        if (seen_patterns.set(&as<SVGPatternElement>(*element)) != AK::HashSetResult::InsertedNewEntry)
            return {};
        return &as<SVGPatternElement>(*element);
    }
    return {};
}

}
