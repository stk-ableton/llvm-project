//===--- PrefixOptionalsCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PrefixOptionalsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/StringRef.h"
#include <cctype>
#include <string>

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

namespace {

bool hasPrefix(const llvm::StringRef &Str, const std::string &Prefix) {
  return Str.size() > Prefix.length() && Str.starts_with(Prefix) &&
         std::isupper(Str[Prefix.length()]);
}

std::string newName(const llvm::StringRef &OldName) {
  if (hasPrefix(OldName, "maybe") || hasPrefix(OldName, "Maybe")) {
    return std::string{"o"} + OldName.substr(5).str();
  }

  if (hasPrefix(OldName, "mMaybe")) {
    return std::string{"mo"} + OldName.substr(6).str();
  }

  if (hasPrefix(OldName, "m")) {
    return std::string{"mo"} + OldName.substr(1).str();
  }

  if (hasPrefix(OldName, "s")) {
    return std::string{"so"} + OldName.substr(1).str();
  }

  return "o" + std::string{static_cast<char>(std::toupper(OldName[0]))} +
         OldName.substr(1).str();
}

} // namespace

PrefixOptionalsCheck::PrefixOptionalsCheck(StringRef Name,
                                           ClangTidyContext *Context)
    : RenamerClangTidyCheck(Name, Context) {}

PrefixOptionalsCheck::~PrefixOptionalsCheck() = default;

std::optional<RenamerClangTidyCheck::FailureInfo>
PrefixOptionalsCheck::getDeclFailureInfo(const NamedDecl *Decl,
                                         const SourceManager &SM) const {
  const auto Kind = Decl->getKind();
  if (Kind == Decl::Kind::ParmVar || Kind == Decl::Kind::Var ||
      Kind == Decl::Kind::Field) {
    auto Type = static_cast<const ValueDecl *>(Decl)->getType();
    if (const auto *TS = Type->getAs<TemplateSpecializationType>()) {
      if (const auto *TD = TS->getTemplateName().getAsTemplateDecl()) {
        // First use the fast getName() method to avoid unnecessary calls to the
        // slow getQualifiedNameAsString().
        if (TD->getName() == "optional" &&
            TD->getQualifiedNameAsString() == "ableton::estd::optional") {
          const auto Name = Decl->getName();
          if (!(hasPrefix(Name, "o") || hasPrefix(Name, "mo") ||
                hasPrefix(Name, "so"))) {
            const auto OldName = Decl->getName().str();
            return FailureInfo{"x", newName(OldName)};
          }
        }
      }
    }
  }
  return {};
}

std::optional<RenamerClangTidyCheck::FailureInfo>
PrefixOptionalsCheck::getMacroFailureInfo(const Token &,
                                          const SourceManager &) const {
  return {};
}

RenamerClangTidyCheck::DiagInfo
PrefixOptionalsCheck::getDiagInfo(const NamingCheckId &ID,
                                  const NamingCheckFailure &Failure) const {
  return DiagInfo{"optional variable for '%0' should have 'o' prefix",
                  [&](DiagnosticBuilder &Diag) { Diag << ID.second; }};
}

} // namespace clang::tidy::misc
