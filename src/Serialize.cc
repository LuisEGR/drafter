//
//  Serialize.cc
//  drafter
//
//  Created by Zdenek Nemec on 5/3/13.
//  Copyright (c) 2013 Apiary Inc. All rights reserved.
//

#include "Serialize.h"
#include "StringUtility.h"

#include <cstdlib>
#include <regex>

using namespace drafter;
using namespace refract;

const std::string SerializeKey::Metadata = "metadata";
const std::string SerializeKey::Reference = "reference";
const std::string SerializeKey::Id = "id";
const std::string SerializeKey::Name = "name";
const std::string SerializeKey::Description = "description";
const std::string SerializeKey::DataStructure = "dataStructure";
const std::string SerializeKey::DataStructures = "dataStructures";
const std::string SerializeKey::ResourceGroup = "resourceGroup";
const std::string SerializeKey::ResourceGroups = "resourceGroups";
const std::string SerializeKey::Resource = "resource";
const std::string SerializeKey::Resources = "resources";
const std::string SerializeKey::URI = "uri";
const std::string SerializeKey::URITemplate = "uriTemplate";
const std::string SerializeKey::Assets = "assets";
const std::string SerializeKey::Actions = "actions";
const std::string SerializeKey::Action = "action";
const std::string SerializeKey::Relation = "relation";
const std::string SerializeKey::Attributes = "attributes";
const std::string SerializeKey::Method = "method";
const std::string SerializeKey::Examples = "examples";
const std::string SerializeKey::Requests = "requests";
const std::string SerializeKey::Responses = "responses";
const std::string SerializeKey::Body = "body";
const std::string SerializeKey::Schema = "schema";
const std::string SerializeKey::Headers = "headers";
const std::string SerializeKey::Model = "model";
const std::string SerializeKey::Value = "value";
const std::string SerializeKey::Parameters = "parameters";
const std::string SerializeKey::Type = "type";
const std::string SerializeKey::Required = "required";
const std::string SerializeKey::Default = "default";
const std::string SerializeKey::Enumerations = "enumerations";
const std::string SerializeKey::Nullable = "nullable";
const std::string SerializeKey::Example = "example";
const std::string SerializeKey::Values = "values";

const std::string SerializeKey::Source = "source";
const std::string SerializeKey::Resolved = "resolved";

const std::string SerializeKey::Element = "element";
const std::string SerializeKey::Role = "role";

const std::string SerializeKey::Version = "_version";
const std::string SerializeKey::Ast = "ast";
const std::string SerializeKey::Sourcemap = "sourcemap";
const std::string SerializeKey::Error = "error";
const std::string SerializeKey::Warning = "warning";
const std::string SerializeKey::Warnings = "warnings";
const std::string SerializeKey::AnnotationCode = "code";
const std::string SerializeKey::AnnotationMessage = "message";
const std::string SerializeKey::AnnotationLocation = "location";
const std::string SerializeKey::AnnotationLocationIndex = "index";
const std::string SerializeKey::AnnotationLocationLength = "length";

const std::string SerializeKey::Variable = "variable";
const std::string SerializeKey::Content = "content";
const std::string SerializeKey::Meta = "meta";
const std::string SerializeKey::Title = "title";
const std::string SerializeKey::Classes = "classes";
const std::string SerializeKey::Samples = "samples";
const std::string SerializeKey::TypeAttributes = "typeAttributes";
const std::string SerializeKey::Optional = "optional";
const std::string SerializeKey::Fixed = "fixed";
const std::string SerializeKey::FixedType = "fixedType";
const std::string SerializeKey::True = "true";
const std::string SerializeKey::Generic = "generic";
const std::string SerializeKey::Enum = "enum";
const std::string SerializeKey::Ref = "ref";
const std::string SerializeKey::Href = "href";
const std::string SerializeKey::Path = "path";

const std::string SerializeKey::Category = "category";
const std::string SerializeKey::Copy = "copy";
const std::string SerializeKey::API = "api";
const std::string SerializeKey::User = "user";
const std::string SerializeKey::Transition = "transition";
const std::string SerializeKey::HrefVariables = "hrefVariables";
const std::string SerializeKey::HTTPHeaders = "httpHeaders";
const std::string SerializeKey::HTTPTransaction = "httpTransaction";
const std::string SerializeKey::ContentType = "contentType";
const std::string SerializeKey::HTTPResponse = "httpResponse";
const std::string SerializeKey::HTTPRequest = "httpRequest";
const std::string SerializeKey::StatusCode = "statusCode";
const std::string SerializeKey::Asset = "asset";
const std::string SerializeKey::MessageBody = "messageBody";
const std::string SerializeKey::MessageBodySchema = "messageBodySchema";
const std::string SerializeKey::Data = "data";

const std::string SerializeKey::ParseResult = "parseResult";
const std::string SerializeKey::Annotation = "annotation";
const std::string SerializeKey::SourceMap = "sourceMap";

using namespace drafter;

template <>
std::pair<bool, dsd::Boolean> drafter::LiteralTo<dsd::Boolean>(const mson::Literal& literal)
{
    if (literal == "true" || literal == "false") {
        return std::make_pair(true, dsd::Boolean{ literal == SerializeKey::True });
    }
    return std::make_pair(false, dsd::Boolean{});
}

namespace
{
    ///
    /// Regular expression matching a ECMA-404 number.
    ///
    /// > A number is a sequence of decimal digits with no superfluous
    /// > leading zero. It may have a preceding minus sign (U+002D).
    /// > It may have a fractional part prefixed by a decimal point (U+002E).
    /// > It may have an exponent, prefixed by e (U+0065) or E (U+0045) and
    /// > optionally + (U+002B) or – (U+002D) . The digits are the code points
    /// > U+0030 through U+0039.
    ///
    // clang-format off
    const std::regex json_number_expression(R"REGEX(-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?)REGEX");
    // clang-format on

    template <typename It>
    bool isValidNumber(It b, It e)
    {
        return std::regex_match(b, e, json_number_expression);
    }
} // namespace

template <>
std::pair<bool, dsd::Number> drafter::LiteralTo<dsd::Number>(const mson::Literal& literal)
{
    using std::begin;
    using std::end;

    // ignore spaces to the right
    auto match_end = end(literal);
    for (; match_end != begin(literal); --match_end) {
        if (!std::isspace(*(match_end - 1)))
            break;
    }

    if (isValidNumber(begin(literal), match_end)) {
        return { true, dsd::Number{ std::string(begin(literal), match_end) } };
    }

    return { false, dsd::Number{} };
}

template <>
std::pair<bool, dsd::String> drafter::LiteralTo<dsd::String>(const mson::Literal& literal)
{
    return std::make_pair(!literal.empty(), dsd::String{ literal });
}
