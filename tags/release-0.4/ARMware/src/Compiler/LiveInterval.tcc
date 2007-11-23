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

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  template<bool T_merge_non_overlap_range>
  void
  LiveInterval::merge_ranges()
  {
    std::sort(m_range.begin(), m_range.end());
    
#if CHECK_CHUNK_CODE
    {
      std::vector<LiveRange>::const_iterator iter = m_range.begin();
      
      // :NOTE: Wei 2005-Feb-05:
      //
      // I want to compare the one with its previous one,
      // so that I can loop to the end.
      ++iter;
      
      while (iter != m_range.end())
      {
        assert((*iter).m_start >= (*(iter - 1)).m_start);
        
        ++iter;
      }
    }
#endif
    
    uint32_t i = 0;
    
    std::vector<LiveRange>::const_iterator iter = m_range.begin();
    
    while (iter != m_range.end())
    {
      if (i < (*iter).m_start)
      {
        i = (*iter).m_end + 1;
        
        m_merged_range.push_back(LiveRange((*iter).m_start, (*iter).m_end));
      }
      else if ((i >= (*iter).m_start) && (i <= (*iter).m_end))
      {
        if (true == T_merge_non_overlap_range)
        {
          assert((i == (*iter).m_start) || (i == ((*iter).m_start + 1)));
        }
        
        if (0 == m_merged_range.size())
        {
          assert((0 == i) && (0 == (*iter).m_start));
          
          m_merged_range.push_back(LiveRange((*iter).m_start, (*iter).m_end));
        }
        else
        {
          m_merged_range.back().m_end = (*iter).m_end;
        }
        
        i = (*iter).m_end + 1;
      }
      
      ++iter;
    }
    
#if CHECK_CHUNK_CODE
    {
      std::vector<LiveRange>::const_iterator iter = m_merged_range.begin();
      
      // :NOTE: Wei 2005-Feb-05:
      //
      // I want to compare the one with its previous one,
      // so that I can loop to the end.
      ++iter;
      
      while (iter != m_merged_range.end())
      {
        assert((*iter).m_end >= (*iter).m_start);
        assert((*iter).m_start > (*(iter - 1)).m_start);
        assert((*iter).m_start > (*(iter - 1)).m_end);
        
        ++iter;
      }
    }
#endif
  } 
}

#endif
