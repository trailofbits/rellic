/*
 * Copyright (c) 2019 Trail of Bits, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <clang/AST/RecursiveASTVisitor.h>

#include <unordered_map>

namespace rellic {

class CXXToCDeclVisitor : public clang::RecursiveASTVisitor<CXXToCDeclVisitor> {
 private:
  clang::ASTContext &ast_ctx;
  clang::TranslationUnitDecl *c_tu;

  std::unordered_map<clang::Decl *, clang::Decl *> c_decls;

  clang::QualType GetAsCType(clang::QualType type);

 public:
  CXXToCDeclVisitor(clang::ASTContext &ctx);

  bool shouldVisitTemplateInstantiations() { return true; }

  bool TraverseFunctionTemplateDecl(clang::FunctionTemplateDecl *decl) {
    // Ignore function templates
    return true;
  }

  bool TraverseClassTemplateDecl(clang::ClassTemplateDecl *decl) {
    // Only process class template specializations
    for (auto spec : decl->specializations()) {
      TraverseDecl(spec);
    }
    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *func);
  bool VisitCXXMethodDecl(clang::CXXMethodDecl *method);
  bool VisitRecordDecl(clang::RecordDecl *record);
  bool VisitCXXRecordDecl(clang::CXXRecordDecl *cls);
  bool VisitFieldDecl(clang::FieldDecl *field);
};

}  // namespace rellic