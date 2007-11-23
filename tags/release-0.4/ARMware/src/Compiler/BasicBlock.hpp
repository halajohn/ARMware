// ARMware - an ARM emulator
// Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef BasicBlock_hpp
#define BasicBlock_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <vector>
#include <deque>

#include "../MemoryPool2.hpp"

#include "DAGNode.hpp"

namespace ARMware
{
  class MIR;
  class LIR;
  
  typedef class BasicBlock BasicBlock;
  class BasicBlock : public MemoryPool2<BasicBlock, 1024>
  {
  public:
    
    struct SuccBBInfo
    {
      bool m_fall_throw;
      BasicBlock *mp_bb;
      
      // Life cycle
      
      inline
      SuccBBInfo(bool const fall_throw, BasicBlock * const bb)
        : m_fall_throw(fall_throw),
          mp_bb(bb)
      { }
      
      // Inquiry
      
      inline BasicBlock *
      bb() const
      { return mp_bb; }
      
      inline bool
      fall_throw() const
      { return m_fall_throw; }
      
      // Access
      
      inline void
      set_bb(BasicBlock * const bb)
      { mp_bb = bb; }
    };
    typedef struct SuccBBInfo SuccBBInfo;
    
  private:
    
    BasicBlock *mp_real_bb;
    
    // :NOTE: Wei 2004-Nov-6:
    //
    // Basic blocks which are not part of the control flow, but for assistant usage.
    //
    // Ex: the jump table basic block for the switch statement.
    std::vector<BasicBlock *> m_assistant_bb;
    
    std::vector<SuccBBInfo> m_succ_bb; // successor
    
    // :NOTE: Wei 2005-May-04:
    //
    // In the LIR emission stage,
    // I may add new basic blocks,
    // thus these blocks should be linking.
    std::vector<BasicBlock *> m_pending_need_link_bb;
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // The following fields are used in the Lengauer-Tarjan algorithm for
    // calculating the dominator tree.
    std::vector<BasicBlock *> m_pred_bb; // predecessor
    std::deque<BasicBlock *> m_bucket;
    
    uint32_t m_df_number;
    BasicBlock *mp_dfs_prev_number_bb;
    BasicBlock *mp_dfs_parent;
    
    BasicBlock *mp_idom_bb;
    std::vector<BasicBlock *> m_be_dominanced_by_me;
    std::vector<BasicBlock *> m_dominance_frontier;
    
    MIR *mp_mir_head;
    MIR *mp_mir_tail;
    
    // :NOTE: Wei 2005-May-10:
    //
    // Because I will add new dag nodes to this stl container
    // (never removal, because I just set the is_usable field to false for a dead dag node)
    // while I am do some processing for the nodes in this container.
    // To not invalidate the iterator, I have to use std::list rather than
    // std::vector or std::deque or something else.
    // Because std::list has an important attribute,
    // if I don't remove the element which the iterator currently points to,
    // then that iterator will still be valid after add and remove operations.
    std::list<DAGNode *> m_all_dag_nodes;
    
    DAGNode *mp_dag_node_head;
    DAGNode *mp_dag_node_tail;
    
    // :NOTE: Wei 2005-May-20:
    //
    // The layout of LIRs in a basic block:
    //
    // mp_lir_head            -->    memory var phi node
    //                               memory var phi node
    // mp_lir_scalar_phi_head -->    scalar var phi node
    //                               scalar var phi node
    // mp_lir_normal_head     -->    normal LIR instruction
    //                               normal LIR instruction
    // mp_lir_tail            -->    normal LIR instruction
    
    LIR *mp_lir_head;
    LIR *mp_lir_scalar_phi_head;
    LIR *mp_lir_normal_head;     // Excluding the phi-instruction.
    LIR *mp_lir_tail;
    
    uint32_t m_next_lir_is_leader;
    
    Label *mp_back_patch_bb_label;
    
    enum TranslationStatusEnum
    {
      TRANSLATION_NONE,
      TRANSLATION_TO_LIR,
      TRANSLATION_DONE
    };
    typedef enum TranslationStatusEnum TranslationStatusEnum;
    
    TranslationStatusEnum m_translation_status;
    
    bool m_finish_emit_native_code;
    bool m_computing_live_interval;
    bool m_already_added_to_the_bfs_list;
    
    bool m_scc_visited;
    
    uint32_t m_scc_dfs_num;
    uint32_t m_scc_low_num;
    
    uint32_t m_postorder_idx;
    
    BasicBlock *mp_prev_postorder_bb;
    BasicBlock *mp_next_postorder_bb;
    
    BasicBlock *mp_next_scc_bb;
    
    // :NOTE: Wei 2004-Oct-25:
    //
    // The following fields are used in live variable analysis (worklist algorithm).
    BasicBlock *mp_next_worklist_bb;
    bool m_in_worklist;
    
    std::vector<uint32_t> m_def;
    std::vector<uint32_t> m_use;
    std::vector<uint32_t> m_in;
    std::vector<uint32_t> m_out;
    
    // :NOTE: Wei 2005-May-24:
    //
    // Used in fully static redundancy elimination.
    std::vector<uint32_t> m_orig_in;
    
    std::vector<DAGNode *> m_dag_node_MOV_table;
    std::vector<DAGNode *> m_dag_node_NOT_table;
    std::vector<DAGNode *> m_dag_node_LOAD_table;
    std::vector<DAGNode *> m_dag_node_ADD_table;
    std::vector<DAGNode *> m_dag_node_AND_table;
    std::vector<DAGNode *> m_dag_node_OR_table;
    std::vector<DAGNode *> m_dag_node_EOR_table;
    std::vector<DAGNode *> m_dag_node_SUB_table;
    std::vector<DAGNode *> m_dag_node_MUL32_table;
    std::vector<DAGNode *> m_dag_node_UMUL64_table;
    std::vector<DAGNode *> m_dag_node_SMUL64_table;
    std::vector<DAGNode *> m_dag_node_LSFT_LEFT_table;
    std::vector<DAGNode *> m_dag_node_LSFT_RIGHT_table;
    std::vector<DAGNode *> m_dag_node_ASFT_RIGHT_table;
    std::vector<DAGNode *> m_dag_node_ROR_table;
    std::vector<DAGNode *> m_dag_node_ADD64_table;
    std::vector<DAGNode *> m_dag_node_TST_table;
    std::vector<DAGNode *> m_dag_node_CMP_table;
    
    uint32_t m_native_buffer_offset;
    
#if PRINT_CHUNK_CODE
    uint32_t m_sequence_idx;
#endif
    
#if CHECK_CHUNK_CODE
    bool m_already_check_ssa_form;
#endif
    
    friend class MemoryPool2<BasicBlock, 1024>;
    
    // Operation
    
    inline DAGNode *
    add_new_dag_node(DAGNode * const node)
    {
      assert(node != 0);
      
      m_all_dag_nodes.push_back(node);
      
      return node;
    }
    
    void add_child_node(DAGNode * const node, DAGNode * const child_node, Variable * const child_var) const;
    
    void prepend_temp_lir_to_real_lir();
    void reset_temp_lir();
    
    void unlink_dag_node(DAGNode * const node);
    
    template<bool T_clear_childs_ancestor, bool T_minus_child_ref_cnt, bool T_preform_unlink>
    void remove_dead_node(DAGNode * const node);
    
    bool dead_code_elimination_internal(DAGNode * const node);
    bool dead_code_elimination_real();
    void dead_code_elimination();
    
    enum ConstantFoldingType
    {
      NEED_MOVE,
      NO_NEED_MOVE
    };
    typedef enum ConstantFoldingType ConstantFoldingType;
    
    ConstantFoldingType determine_constant_folding_type(DAGNode * const node);
    
    void replace_execution_order(DAGNode * const node, DAGNode * const to_position);
    
    template<typename T_dag_type>
    void constant_folding_internal_type_cc(DAGNode * const node,
                                           ChildInfo const &child_0,
                                           ChildInfo const &child_1);
    
    template<typename T_dag_type>
    void constant_folding_internal_type_xcc(DAGNode * const node,
                                            ChildInfo const &child_0,
                                            ChildInfo const &child_1,
                                            ChildInfo const &child_0_child_0,
                                            ChildInfo const &child_0_child_1);
    
    template<typename T_dag_type>
    bool constant_folding_internal(DAGNode * const node);
    bool constant_folding_real(DAGNode * const node);
    void constant_folding();
    
    template<VarAttachedTypeEnum T_attach>
    void split_node(DAGNode * const node);
    
    void partition_dag_to_trees_internal(DAGNode * const node);
    void partition_dag_to_trees();
    
    void emission_lir_for_each_node_internal(DAGNode * const node);
    void emission_lir_for_each_node();
    
    template<DAGKindEnum T_kind>
    void emit_lir_code(DAGNode * const node);
    
    template<typename T_dag_node>
    std::vector<DAGNode *> &determine_dag_node_table();
    
    template<typename T_dag_node>
    DAGNode *find_dag_node(DAGNode * const child);
    
    template<typename T_dag_node, bool operand_exchangable>
    DAGNode *find_dag_node(std::vector<Variable *> &child_var,
                           DAGNode * const child_1,
                           DAGNode * const child_2);
    
    template<typename T_dag_node, bool operand_exchangable>
    DAGNode *find_dag_node(std::vector<Variable *> &child_var,
                           DAGNode * const child_1,
                           DAGNode * const child_2,
                           DAGNode * const child_3,
                           DAGNode * const child_4);
    
    void link_nodes(DAGNode * const node);
    
    template<typename T_mir>
    void add_CALL_0_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_CALL_1_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_LOAD_LABEL_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_RETURN_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_RETRIEVE_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_ADD_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_ADC_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_MOV_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_UMUL64_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_ADD64_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_CMP_EQ_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_GOTO_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_PRODUCE_CBIT_type_mir_into_dag_internal(MIR * const mir);
    
    template<typename T_mir>
    void add_CLEAR_CBIT_type_mir_into_dag_internal(MIR * const mir);
    
    void add_mir_into_dag(MIR * const mir);
    void translate_dag_into_lir();
    BasicBlock *determine_back_patch_bb();
    void relink_possible_added_bb();
    void translate_MIR_to_LIR_internal();
    
#if PRINT_CHUNK_CODE
    void dump_dag_tree(DAGNode * const node, uint32_t const level) const;
#endif
    
#if CHECK_CHUNK_CODE || PRINT_CHUNK_CODE
    template<CheckAncestorNumberTiming T_timing>
    void dump_or_check_on_tree() const;
#endif
    
  public:
    
#if PRINT_CHUNK_CODE
    static uint32_t BB_SEQUENCE_IDX;
#endif
    
    // Life cycle
    
    BasicBlock(MIR * const mir_head)
      : mp_real_bb(this),
        // :Lenguaer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
        //
        // step1:
        //   for each v belongs to V do prev(v) := empty; semi(v) := 0 od;
        //
        // And here, I will use 'm_df_number' as the initial semi value.
        m_df_number(0),
        
        mp_idom_bb(0),
        mp_mir_head(mir_head),
        mp_dag_node_head(0),
        mp_dag_node_tail(0),
        mp_lir_head(0),
        mp_lir_scalar_phi_head(0),
        mp_lir_normal_head(0),
        m_next_lir_is_leader(false),
        mp_back_patch_bb_label(0),
        m_translation_status(TRANSLATION_NONE),
        m_finish_emit_native_code(false),
        m_computing_live_interval(false),
        m_already_added_to_the_bfs_list(false),
        m_scc_visited(false),
        m_postorder_idx(0),
#if CHECK_CHUNK_CODE
        mp_prev_postorder_bb(0),
        mp_next_postorder_bb(0),
#endif
        mp_next_scc_bb(0),
        m_in_worklist(false)
#if PRINT_CHUNK_CODE
      , m_sequence_idx(BB_SEQUENCE_IDX++)
#endif
#if CHECK_CHUNK_CODE
      , m_already_check_ssa_form(false)
#endif
    { }
    
    ~BasicBlock();
    
    // Operation
    
    DAGNode *find_var_dag_node(Variable * const variable);
    
    inline void
    add_assistant_bb(BasicBlock * const bb)
    {
      assert(bb != 0);
      assert(bb != this);
      
      m_assistant_bb.push_back(bb);
    }
    
    inline void
    add_succ_bb(bool const fall_throw, BasicBlock * const bb)
    {
      assert(bb != 0);
      assert(bb != this);
      
#if CHECK_CHUNK_CODE
      for (std::vector<SuccBBInfo>::const_iterator iter = m_succ_bb.begin();
           iter != m_succ_bb.end();
           ++iter)
      {
        assert((*iter).bb() != bb);
      }
#endif
      
      m_succ_bb.push_back(SuccBBInfo(fall_throw, bb));
    }
    
    void replace_succ_bb_and_patch_last_jmp_label_if_possible(BasicBlock * const old_bb,
                                                              BasicBlock * const new_bb);
    
    inline void
    add_succ_true_false_bb(BasicBlock * const true_bb, BasicBlock * const false_bb)
    {
      assert(true_bb != 0);
      assert(false_bb != 0);
      
      // :NOTE: Wei 2004-Oct-19:
      //
      // Due to the preformance of runtime, we need to make a consistent such that
      // we push the fall throw block (false_bb) first, and then the true_bb.
      add_succ_bb(true, false_bb);
      add_succ_bb(false, true_bb);
    }
    
    inline void
    add_pred_bb(BasicBlock * const bb)
    {
      assert(bb != 0);
      assert(bb != this);
      
#if CHECK_CHUNK_CODE
      for (std::vector<BasicBlock *>::const_iterator iter = m_pred_bb.begin();
           iter != m_pred_bb.end();
           ++iter)
      {
        assert((*iter) != bb);
      }
#endif
      
      m_pred_bb.push_back(bb);
    }
    
    inline void
    replace_pred_bb(BasicBlock * const old_bb, BasicBlock * const new_bb)
    {
      assert(old_bb != this);
      assert(new_bb != this);
      assert(old_bb != new_bb);
      
      for (std::vector<BasicBlock *>::iterator iter = m_pred_bb.begin();
           iter != m_pred_bb.end();
           ++iter)
      {
        if ((*iter) == old_bb)
        {
          (*iter) = new_bb;
          
          return;
        }
      }
      
      assert(!"Should not reach here.");
    }
    
    inline void
    add_to_bucket(BasicBlock * const bb)
    { m_bucket.push_back(bb); }
    
    inline BasicBlock *
    remove_from_bucket()
    {
      // :NOTE: Wei 2004-Oct-20:
      //
      // If we want to use st::deque::front(), we need to ensure
      // that the deque must not be empty.
      if (false == m_bucket.empty())
      {
        BasicBlock * const bb = m_bucket.front();
        
        m_bucket.pop_front();
        
        return bb;
      }
      else
      {
        return 0;
      }
    }
    
    void translate_MIR_to_LIR();
    
    inline bool
    is_already_added_to_the_bfs_list() const
    { return m_already_added_to_the_bfs_list; }
    
    inline bool
    is_scc_visited() const
    { return m_scc_visited; }
    
    inline uint32_t
    scc_dfs_num() const
    { return m_scc_dfs_num; }
    
    inline uint32_t
    scc_low_num() const
    { return m_scc_low_num; }
    
    inline uint32_t
    postorder_idx() const
    { return m_postorder_idx; }
    
    inline BasicBlock *
    prev_postorder_bb() const
    { return mp_prev_postorder_bb; }
    
    inline BasicBlock *
    next_postorder_bb() const
    { return mp_next_postorder_bb; }
    
    inline BasicBlock *
    next_scc_bb() const
    { return mp_next_scc_bb; }
    
    inline bool
    is_finish_emit_native_code() const
    { return m_finish_emit_native_code; }
    
    inline bool
    is_computing_live_interval()
    { return m_computing_live_interval; }
    
    void add_phi_node_internal(LIR * const phi, LIR *lir_head);
    void add_phi_node(Variable * const var);
    
    void append_lir_internal(LIR * const lir);
    
    template<bool T_need_load_store>
    LIR *append_lir(LIR * const lir);
    
    void insert_lir_before_the_last_jmp(LIR * const lir);
    
    inline void
    set_mir_tail(MIR * const mir)
    { mp_mir_tail = mir; }
    
    template<ConsiderMemoryVarEnum T_consider>
    void calculate_def_use(uint32_t const var_number);
    
    bool calculate_in_out();
    
    template<bool T_root_bb>
    bool calculate_AVAIL_in_out();
    
    bool is_var_live_in(Variable * const var);
    
    inline void
    add_be_dominanced(BasicBlock * const bb)
    { m_be_dominanced_by_me.push_back(bb); }
    
    void rename_variable();
    uint32_t pred_bb_idx(BasicBlock * const bb);
    void fill_ssa_phi_function_operand(BasicBlock * const bb);
    
    template<DefUseEnum T_type>
    void set_def_use(Variable * const var);
    
    void set_AVAIL_AVLOC(Variable * const var);
    void set_AVAIL_AVLOC(uint32_t const memory_operation_idx);
    void calculate_AVAIL_AVLOC(uint32_t const var_number);
    
#if CHECK_CHUNK_CODE
    void check_label_linked() const;
#endif
    
#if PRINT_CHUNK_CODE
    template<bool T_ssa_enabled, bool T_use_global_idx, bool T_print_memory_operation_idx>
    void dump_lir_info() const;
    
    void dump_info() const;
    void dump_dominance_frontier();
#endif
    
    // Access
    
    inline void
    set_lir_head(LIR * const lir)
    { mp_lir_head = lir; }
    
    inline void
    set_lir_scalar_phi_head(LIR * const lir)
    { mp_lir_scalar_phi_head = lir; }
    
    inline void
    set_lir_normal_head(LIR * const lir)
    { mp_lir_normal_head = lir; }
    
    inline void
    set_lir_tail(LIR * const lir)
    { mp_lir_tail = lir; }
    
    inline std::vector<uint32_t> &
    in()
    { return m_in; }
    
    inline std::vector<uint32_t> &
    orig_in()
    { return m_orig_in; }
    
    inline void
    set_translation_status(TranslationStatusEnum const status)
    { m_translation_status = status; }
    
    inline void
    set_real_bb(BasicBlock * const bb)
    { mp_real_bb = bb; }
    
    inline void
    set_native_buffer_offset(uint32_t const offset)
    { m_native_buffer_offset = offset; }
    
    inline void
    back_patch_bb_label_use_next_lir(Label * const label)
    {
      assert(0 == mp_back_patch_bb_label);
      assert(Label::BASIC_BLOCK == label->kind());
      
      mp_back_patch_bb_label = label;
    }
    
    inline void
    set_df_number(uint32_t const num)
    { m_df_number = num; }
    
    inline void
    set_dfs_prev_number_bb(BasicBlock * const bb)
    { mp_dfs_prev_number_bb = bb; }
    
    inline void
    set_dfs_parent(BasicBlock * const bb)
    { mp_dfs_parent = bb; }
    
    inline void
    set_idom_bb(BasicBlock * const bb)
    { mp_idom_bb = bb; }
    
    inline void
    set_next_worklist_bb(BasicBlock * const bb)
    { mp_next_worklist_bb = bb; }
    
    inline void
    set_in_worklist(bool const in)
    { m_in_worklist = in; }
    
    inline std::vector<SuccBBInfo> &
    succ_bb()
    { return m_succ_bb; }
    
    inline std::vector<BasicBlock *> &
    pred_bb()
    { return m_pred_bb; }
    
    inline std::vector<BasicBlock *> &
    dominance_frontier()
    { return m_dominance_frontier; }
    
    inline std::vector<BasicBlock *> &
    dominanced_by_me()
    { return m_be_dominanced_by_me; }
    
    inline void
    set_finish_emit_native_code()
    { m_finish_emit_native_code = true; }
    
    inline void
    set_computing_live_interval()
    { m_computing_live_interval = true; }
    
    inline void
    set_already_added_to_the_bfs_list()
    { m_already_added_to_the_bfs_list = true; }
    
    inline void
    set_postorder_idx(uint32_t const idx)
    { m_postorder_idx = idx; }
    
    inline void
    set_prev_postorder_bb(BasicBlock * const bb)
    { mp_prev_postorder_bb = bb; }
    
    inline void
    set_next_postorder_bb(BasicBlock * const bb)
    { mp_next_postorder_bb = bb; }
    
    inline void
    set_next_scc_bb(BasicBlock * const bb)
    { mp_next_scc_bb = bb; }
    
    inline void
    set_scc_visited()
    { m_scc_visited = true; }
    
    inline void
    set_scc_dfs_num(uint32_t const num)
    { m_scc_dfs_num = num; }
    
    inline void
    set_scc_low_num(uint32_t const num)
    { m_scc_low_num = num; }
    
#if PRINT_CHUNK_CODE
    inline void
    set_sequence_idx(uint32_t const sequence_idx)
    { m_sequence_idx = sequence_idx; }
#endif
    
#if CHECK_CHUNK_CODE
    inline void
    set_already_check_ssa_form()
    {
      assert(false == m_already_check_ssa_form);
      
      m_already_check_ssa_form = true;
    }
#endif
    
    // Inquiry
    
    inline BasicBlock *
    real_bb() const
    { return mp_real_bb; }
    
    inline Label *
    back_patch_bb_label() const
    {
#if CHECK_CHUNK_CODE
      if (mp_back_patch_bb_label != 0)
      {
        assert(Label::BASIC_BLOCK == mp_back_patch_bb_label->kind());
      }
#endif
      
      return mp_back_patch_bb_label;
    }
    
    inline uint32_t
    native_buffer_offset() const
    { return m_native_buffer_offset; }
    
    inline uint32_t
    df_number() const
    { return m_df_number; }
    
    inline BasicBlock const *
    idom_bb() const
    { return mp_idom_bb; }
    
    inline MIR *
    mir_head() const
    { return mp_mir_head; }
    
    inline MIR *
    mir_tail() const
    { return mp_mir_tail; }
    
    inline BasicBlock *
    dfs_prev_number_bb() const
    { return mp_dfs_prev_number_bb; }
    
    inline BasicBlock *
    dfs_parent() const
    { return mp_dfs_parent; }
    
    inline BasicBlock *
    next_worklist_bb() const
    { return mp_next_worklist_bb; }
    
    inline bool
    in_worklist() const
    { return m_in_worklist; }
    
    inline std::vector<BasicBlock *> const &
    dominanced_by_me() const
    { return m_be_dominanced_by_me; }
    
    inline std::vector<BasicBlock *> const &
    dominance_frontier() const
    { return m_dominance_frontier; }
    
    inline std::vector<SuccBBInfo> const &
    succ_bb() const
    { return m_succ_bb; }
    
    inline std::vector<BasicBlock *> const &
    pred_bb() const
    { return m_pred_bb; }
    
    inline std::vector<uint32_t> const &
    in() const
    { return m_in; }
    
    inline std::vector<uint32_t> const &
    out() const
    { return m_out; }
    
    inline std::vector<BasicBlock *> const &
    assistant_bb() const
    { return m_assistant_bb; }
    
    inline LIR *
    lir_head() const
    { return mp_lir_head; }
    
    inline LIR *
    lir_scalar_phi_head() const
    { return mp_lir_scalar_phi_head; }
    
    inline LIR *
    lir_normal_head() const
    { return mp_lir_normal_head; }
    
    inline LIR *
    lir_tail() const
    { return mp_lir_tail; }
    
    bool is_dominated_by_me(BasicBlock const * const bb);
    
#if CHECK_CHUNK_CODE
    void check_ssa_form();
#endif
    
#if PRINT_CHUNK_CODE
    inline uint32_t
    sequence_idx() const
    { return m_sequence_idx; }
    
    uint32_t start_inst_idx() const;
#endif
    
#if CHECK_CHUNK_CODE
    inline bool
    already_check_ssa_form() const
    { return m_already_check_ssa_form; }
#endif
  };
}

#endif

#endif
