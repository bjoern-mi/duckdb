#include "duckdb/catalog/catalog.hpp"
#include "duckdb/parser/expression/constant_expression.hpp"
#include "duckdb/parser/statement/insert_statement.hpp"
#include "duckdb/parser/query_node/select_node.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/parser/statement/relation_statement.hpp"

using namespace std;

namespace duckdb {

BoundStatement Binder::Bind(RelationStatement &stmt) {
	throw NotImplementedException("FIXME: bind relation stmt");
}

} // namespace duckdb
