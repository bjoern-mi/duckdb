//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/index/art/leaf.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/execution/index/art/prefix.hpp"

namespace duckdb {

// classes
class ART;
class ARTNode;
class Key;
class MetaBlockWriter;
class MetaBlockReader;

// structs
struct BlockPointer;

class Leaf {
public:
	//! Number of row IDs
	uint32_t count;
	//! Compressed path (prefix)
	Prefix prefix;
	union {
		//! The position to the head of the list of leaf segments
		idx_t position;
		//! Inlined row ID
		row_t inlined;
	} row_ids;

public:
	//! Get a new pointer to a node, might cause a new buffer allocation, and initializes a leaf holding one
	//! row ID and a prefix starting at depth
	static Leaf *New(ART &art, ARTNode &node, const Key &key, const uint32_t depth, const row_t row_id);
	//! Get a new pointer to a node, might cause a new buffer allocation, and initializes a leaf holding
	//! n_row_ids row IDs and a prefix starting at depth
	static Leaf *New(ART &art, ARTNode &node, const Key &key, const uint32_t depth, const row_t *row_ids,
	                 const idx_t count);
	//! Free the leaf
	static void Free(ART &art, ARTNode &node);

	//! Initializes a merge by incrementing the buffer IDs of the leaf segments
	void InitializeMerge(const ART &art, const idx_t buffer_count);
	//! Merge leaves
	void Merge(ART &art, ARTNode &other);

	//! Insert a row ID into a leaf
	void Insert(ART &art, const row_t row_id);
	//! Remove a row ID from a leaf
	void Remove(ART &art, const row_t row_id);

	//! Returns whether this leaf is inlined
	inline bool IsInlined() const {
		return count <= 1;
	}
	//! Get the row ID at the position
	row_t GetRowId(const ART &art, const idx_t position) const;
	//! Returns the position of a row ID, and an invalid index, if the leaf does not contain the row ID
	uint32_t FindRowId(const ART &art, idx_t &position, const row_t row_id) const;

	//! Returns the string representation of a leaf
	string ToString(const ART &art) const;

	//! Serialize this leaf
	BlockPointer Serialize(const ART &art, MetaBlockWriter &writer) const;
	//! Deserialize this leaf
	void Deserialize(ART &art, MetaBlockReader &reader);

	//! Vacuum the leaf segments of a leaf, if not inlined
	void Vacuum(ART &art);

private:
	//! Moves the inlined row ID onto a leaf segment, does not change the size
	//! so this will be a (temporarily) invalid leaf
	void MoveInlinedToSegment(ART &art);
};

} // namespace duckdb
