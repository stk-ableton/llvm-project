//===--- PrefixOptionalsCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_PREFIXOPTIONALSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_PREFIXOPTIONALSCHECK_H

#include "../utils/RenamerClangTidyCheck.h"

namespace clang::tidy::misc {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc/prefix-optionals.html
class PrefixOptionalsCheck : public RenamerClangTidyCheck {
public:
  PrefixOptionalsCheck(StringRef Name, ClangTidyContext *Context);
  ~PrefixOptionalsCheck();

  std::optional<FailureInfo>
  getDeclFailureInfo(const NamedDecl *Decl,
                     const SourceManager &SM) const override;

  std::optional<FailureInfo>
  getMacroFailureInfo(const Token &MacroNameTok,
                      const SourceManager &SM) const override;

  DiagInfo getDiagInfo(const NamingCheckId &ID,
                       const NamingCheckFailure &Failure) const override;
};

} // namespace clang::tidy::misc

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_PREFIXOPTIONALSCHECK_H
