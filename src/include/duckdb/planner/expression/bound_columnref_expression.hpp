//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/expression/bound_columnref_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/column_binding.hpp"
#include "duckdb/planner/expression.hpp"

namespace duckdb {

//! A BoundColumnRef expression represents a ColumnRef expression that was bound to an actual table and column index. It
//! is not yet executable, however. The ColumnBindingResolver transforms the BoundColumnRefExpressions into
//! BoundExpressions, which refer to indexes into the physical chunks that pass through the executor.
class BoundColumnRefExpression : public Expression {
public:
	BoundColumnRefExpression(string alias, TypeId type, SQLType sql_type, ColumnBinding binding, idx_t depth = 0);
	BoundColumnRefExpression(TypeId type, SQLType sql_type, ColumnBinding binding, idx_t depth = 0)
		: BoundColumnRefExpression(string(), type, move(sql_type), binding, depth) {
	}
	BoundColumnRefExpression(SQLType sql_type, ColumnBinding binding, idx_t depth = 0) :
		BoundColumnRefExpression(GetInternalType(sql_type), move(sql_type), move(binding), depth) {}
	BoundColumnRefExpression(string alias, SQLType sql_type, ColumnBinding binding, idx_t depth = 0) :
		BoundColumnRefExpression(move(alias), GetInternalType(sql_type), move(sql_type), move(binding), depth) {}

	//! Column index set by the binder, used to generate the final BoundExpression
	ColumnBinding binding;
	//! The subquery depth (i.e. depth 0 = current query, depth 1 = parent query, depth 2 = parent of parent, etc...).
	//! This is only non-zero for correlated expressions inside subqueries.
	idx_t depth;

public:
	bool IsScalar() const override {
		return false;
	}
	bool IsFoldable() const override {
		return false;
	}

	string ToString() const override;

	bool Equals(const BaseExpression *other) const override;
	hash_t Hash() const override;

	unique_ptr<Expression> Copy() override;
};
} // namespace duckdb
