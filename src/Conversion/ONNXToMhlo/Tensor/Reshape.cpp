/*
 * SPDX-License-Identifier: Apache-2.0
 */

//===---------------- Reshape.cpp - Lowering Reshape Op -------------------===//
//
// Copyright 2022
//
// =============================================================================
//
// This file lowers the ONNX Reshape Operator to Mhlo dialect.
//
//===----------------------------------------------------------------------===//

#include "src/Conversion/ONNXToMhlo/ONNXToMhloCommon.hpp"
#include "src/Dialect/ONNX/ONNXOps/ShapeHelper.hpp"

using namespace mlir;

namespace onnx_mlir {

namespace {

struct ONNXReshapeOpLoweringToMhlo : public ConversionPattern {
  ONNXReshapeOpLoweringToMhlo(MLIRContext *ctx)
      : ConversionPattern(mlir::ONNXReshapeOp::getOperationName(), 1, ctx) {}

  LogicalResult matchAndRewrite(Operation *op, ArrayRef<Value> operands,
      ConversionPatternRewriter &rewriter) const final {
    ONNXReshapeOpAdaptor operandAdaptor(operands, op->getAttrDictionary());
    Location loc = op->getLoc();
    Value data = operandAdaptor.getData();
    Value shape = operandAdaptor.getShape();
    Type outputType = *op->result_type_begin();
    Value result =
        rewriter.create<mhlo::DynamicReshapeOp>(loc, outputType, data, shape);
    rewriter.replaceOp(op, result);
    return success();
  }
};

} // namespace

void populateLoweringONNXReshapeOpToMhloPattern(
    RewritePatternSet &patterns, MLIRContext *ctx) {
  patterns.insert<ONNXReshapeOpLoweringToMhlo>(ctx);
}

} // namespace onnx_mlir
