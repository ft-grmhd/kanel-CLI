// Copyright (C) 2024 kanel
// This file is part of "kanel-CLI"
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef KANEL_CLI_UTILS_DATA_STRUCTURES_TREES_RED_BLACK
#define KANEL_CLI_UTILS_DATA_STRUCTURES_TREES_RED_BLACK

#include <stdbool.h>
#include <stdint.h>

#include <Core/CompilationProfile.h>

// I miss C++ tmplates...

#define KANEL_CLI_GENERIC_RED_BLACK_TREE(T) \
	typedef struct KbhRedBlackTreeNode_##T \
	{ \
		struct KbhRedBlackTreeNode_##T* parent; \
		struct KbhRedBlackTreeNode_##T* right; \
		struct KbhRedBlackTreeNode_##T* left; \
		T data; \
		bool is_red; \
	} KbhRedBlackTreeNode_##T; \
\
	static inline KbhRedBlackTreeNode_##T* kbhRedBlackTreeBSTInsert_##T(KbhRedBlackTreeNode_##T* node, KbhRedBlackTreeNode_##T* tmp) \
	{ \
		if(node == KANEL_CLI_NULLPTR) \
			return tmp; \
		if(tmp->data <= node->data) \
		{ \
			node->left = kbhRedBlackTreeBSTInsert_##T(node->left, tmp); \
			node->left->parent = node; \
		} \
		else if(tmp->data > node->data) \
		{ \
			node->right = kbhRedBlackTreeBSTInsert_##T(node->right, tmp); \
			node->right->parent = node; \
		} \
		return node; \
	} \
\
	static inline void kbhRedBlackTreeRightRotate_##T(KbhRedBlackTreeNode_##T* root, KbhRedBlackTreeNode_##T* tmp) \
	{ \
		KbhRedBlackTreeNode_##T* left = tmp->left; \
		tmp->left = left->right; \
		if(tmp->left) \
			tmp->left->parent = tmp; \
		left->parent = tmp->parent; \
		if(!tmp->parent) \
			root = left; \
		else if(tmp == tmp->parent->left) \
			tmp->parent->left = left; \
		else \
			tmp->parent->right = left; \
		left->right = tmp; \
		tmp->parent = left; \
	} \
\
	static inline void kbhRedBlackTreeLeftRotate_##T(KbhRedBlackTreeNode_##T* root, KbhRedBlackTreeNode_##T* tmp) \
	{ \
		KbhRedBlackTreeNode_##T* right = tmp->right; \
		tmp->right = right->left; \
		if(tmp->right) \
			tmp->right->parent = tmp; \
		right->parent = tmp->parent; \
		if(!tmp->parent) \
			root = right; \
		else if(tmp == tmp->parent->left) \
			tmp->parent->left = right; \
		else \
			tmp->parent->right = right; \
		right->left = tmp; \
		tmp->parent = right; \
	} \
\
	static inline void KbhRedBlackTreeFixup_##T(KbhRedBlackTreeNode_##T* root, KbhRedBlackTreeNode_##T* pt) \
	{ \
		KbhRedBlackTreeNode_##T* parent_pt = KANEL_CLI_NULLPTR; \
		KbhRedBlackTreeNode_##T* grand_parent_pt = KANEL_CLI_NULLPTR; \
		while(pt != root && pt->is_red && pt->parent->is_red) \
		{ \
			parent_pt = pt->parent; \
			grand_parent_pt = pt->parent->parent; \
			if(parent_pt == grand_parent_pt->left) \
			{ \
				KbhRedBlackTreeNode_##T* uncle_pt = grand_parent_pt->right; \
				if(uncle_pt != KANEL_CLI_NULLPTR && uncle_pt->is_red) \
				{ \
					grand_parent_pt->is_red = true; \
					parent_pt->is_red = false; \
					uncle_pt->is_red = false; \
					pt = grand_parent_pt; \
				} \
				else \
				{ \
					if(pt == parent_pt->right) \
					{ \
						kbhRedBlackTreeLeftRotate_##T(root, parent_pt); \
					pt = parent_pt; \
						parent_pt = pt->parent; \
					} \
					kbhRedBlackTreeRightRotate_##T(root, grand_parent_pt); \
					bool t = parent_pt->is_red; \
					parent_pt->is_red = grand_parent_pt->is_red; \
					grand_parent_pt->is_red = t; \
					pt = parent_pt; \
				} \
			} \
			else \
			{ \
				KbhRedBlackTreeNode_##T* uncle_pt = grand_parent_pt->left; \
				if(uncle_pt != KANEL_CLI_NULLPTR && uncle_pt->is_red)  \
				{ \
					grand_parent_pt->is_red = true; \
					parent_pt->is_red = false; \
					uncle_pt->is_red = false; \
					pt = grand_parent_pt; \
				} \
				else \
				{ \
					if(pt == parent_pt->left) \
					{ \
						kbhRedBlackTreeRightRotate_##T(root, parent_pt); \
						pt = parent_pt; \
						parent_pt = pt->parent; \
					} \
					kbhRedBlackTreeLeftRotate_##T(root, grand_parent_pt); \
					bool t = parent_pt->is_red; \
					parent_pt->is_red = grand_parent_pt->is_red; \
					grand_parent_pt->is_red = t; \
					pt = parent_pt; \
				} \
			} \
		} \
	}\
\
	static inline KbhRedBlackTreeNode_##T* kbhRedBlackTreeFind_##T(KbhRedBlackTreeNode_##T* root, T value) \
	{ \
		if(root == KANEL_CLI_NULLPTR || root->data == value) \
			return root; \
		if(root->data < value) \
			return kbhRedBlackTreeFind_##T(root->right, value); \
		return kbhRedBlackTreeFind_##T(root->left, value); \
	} \
\
	static inline KbhRedBlackTreeNode_##T* kbhRedBlackTreeFindNodePtr_##T(KbhRedBlackTreeNode_##T* root, KbhRedBlackTreeNode_##T* node) \
	{ \
		if(root == KANEL_CLI_NULLPTR || root == node) \
			return root; \
		if(root->data < node->data) \
			return kbhRedBlackTreeFindNodePtr_##T(root->right, node); \
		return kbhRedBlackTreeFindNodePtr_##T(root->left, node); \
	} \
\
	static inline KbhRedBlackTreeNode_##T* kbhRedBlacktreeGetSuccessor_##T(KbhRedBlackTreeNode_##T* root) \
	{ \
		root = root->right; \
		while(root != KANEL_CLI_NULLPTR && root->left != KANEL_CLI_NULLPTR) \
			root = root->left; \
		return root; \
	} \
\
	static inline KbhRedBlackTreeNode_##T* kbhRedBlackTreeBSTRemoveNode_##T(KbhRedBlackTreeNode_##T* root, KbhRedBlackTreeNode_##T* node, void(*deleter)(KbhRedBlackTreeNode_##T*)) \
	{ \
		if(root == KANEL_CLI_NULLPTR) \
			return root; \
		if(root->data > node->data) \
			root->left = kbhRedBlackTreeBSTRemoveNode_##T(root->left, node, deleter); \
		else if(root->data < node->data) \
			root->right = kbhRedBlackTreeBSTRemoveNode_##T(root->right, node, deleter); \
		else if(root == node) \
		{ \
			if(root->left == KANEL_CLI_NULLPTR) \
			{ \
				KbhRedBlackTreeNode_##T* tmp = root->right; \
				if(deleter != KANEL_CLI_NULLPTR) \
					deleter(root); \
				return tmp; \
			} \
			if(root->right == KANEL_CLI_NULLPTR) \
			{ \
				KbhRedBlackTreeNode_##T* tmp = root->left; \
				if(deleter != KANEL_CLI_NULLPTR) \
					deleter(root); \
				return tmp; \
			} \
			KbhRedBlackTreeNode_##T* succ = kbhRedBlacktreeGetSuccessor_##T(root); \
			root->data = succ->data; \
			root->right = kbhRedBlackTreeBSTRemoveNode_##T(root->right, succ, deleter); \
		} \
		else \
			root->left = kbhRedBlackTreeBSTRemoveNode_##T(root->left, node, deleter); \
		return root; \
	} \
\
	static inline KbhRedBlackTreeNode_##T* kbhRedBlackTreeBSTRemoveValue_##T(KbhRedBlackTreeNode_##T* root, T value, void(*deleter)(KbhRedBlackTreeNode_##T*)) \
	{ \
		if(root == KANEL_CLI_NULLPTR) \
			return root; \
		if(root->data > value) \
			root->left = kbhRedBlackTreeBSTRemoveValue_##T(root->left, value, deleter); \
		else if(root->data < value) \
			root->right = kbhRedBlackTreeBSTRemoveValue_##T(root->right, value, deleter); \
		else \
		{ \
			if(root->left == KANEL_CLI_NULLPTR) \
			{ \
				KbhRedBlackTreeNode_##T* tmp = root->right; \
				if(deleter != KANEL_CLI_NULLPTR) \
					deleter(root); \
				return tmp; \
			} \
			if(root->right == KANEL_CLI_NULLPTR) \
			{ \
				KbhRedBlackTreeNode_##T* tmp = root->left; \
				if(deleter != KANEL_CLI_NULLPTR) \
					deleter(root); \
				return tmp; \
			} \
			KbhRedBlackTreeNode_##T* succ = kbhRedBlacktreeGetSuccessor_##T(root); \
			root->data = succ->data; \
			root->right = kbhRedBlackTreeBSTRemoveValue_##T(root->right, succ->data, deleter); \
		} \
		return root; \
	}

KANEL_CLI_GENERIC_RED_BLACK_TREE(int8_t)
KANEL_CLI_GENERIC_RED_BLACK_TREE(uint8_t)

KANEL_CLI_GENERIC_RED_BLACK_TREE(int16_t)
KANEL_CLI_GENERIC_RED_BLACK_TREE(uint16_t)

KANEL_CLI_GENERIC_RED_BLACK_TREE(int32_t)
KANEL_CLI_GENERIC_RED_BLACK_TREE(uint32_t)

KANEL_CLI_GENERIC_RED_BLACK_TREE(int64_t)
KANEL_CLI_GENERIC_RED_BLACK_TREE(uint64_t)

KANEL_CLI_GENERIC_RED_BLACK_TREE(float)
KANEL_CLI_GENERIC_RED_BLACK_TREE(double)

#endif
