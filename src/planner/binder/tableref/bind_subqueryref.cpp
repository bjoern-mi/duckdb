#include "duckdb/parser/tableref/subqueryref.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/planner/bound_query_node.hpp"

using namespace std;

namespace duckdb {

unique_ptr<LogicalOperator> Binder::Bind(SubqueryRef &ref) {
	Binder subquery_binder(context, this);
	auto subquery = subquery_binder.BindNode(*ref.subquery);
	idx_t bind_index = subquery->GetRootIndex();

	bind_context.AddSubquery(bind_index, ref.alias, ref, *subquery);
	MoveCorrelatedExpressions(subquery_binder);

	subquery_binder.plan_subquery = plan_subquery;
	auto subquery_plan = subquery_binder.CreatePlan(*subquery);
	if (subquery_binder.has_unplanned_subqueries) {
		has_unplanned_subqueries = true;
	}
	return subquery_plan;
}

}
