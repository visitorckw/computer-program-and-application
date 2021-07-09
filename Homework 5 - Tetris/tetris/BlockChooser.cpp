/*
    Bastet - tetris clone with embedded bastard block chooser
    (c) 2005-2009 Federico Poloni <f.polonithirtyseven@sns.it> minus 37

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Block.hpp"
#include "BlockChooser.hpp"

#include <cstdlib> //random

namespace Bastet{

  BlockChooser::BlockChooser(){}
  BlockChooser::~BlockChooser(){}

  NoviceBlockChooser::NoviceBlockChooser(){}
  NoviceBlockChooser::~NoviceBlockChooser(){}
  
  Queue NoviceBlockChooser::GetStartingQueue(){
    Queue q;
    q.push_back(GetNext());
    q.push_back(GetNext());
    return q;
  }
  
  Block* NoviceBlockChooser::GetNext(){
    BlockType bt = BlockType(rand() % 2);
    Block* b = NULL;

    switch (bt) {
    case O:
        b = new OBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
        break;
    case I:
        b = new IBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
        break;
    }    
    return b;
  }
  Block* RandomBlockChooser::GetNext() {
      BlockType b =  BlockType(rand() % 7);
      Block* bPtr = NULL;
      switch (b) {
          case O:
              bPtr = new OBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
              break;
          case I:
              bPtr = new IBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
              break;
          case S:
              bPtr = new SBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
              break;
          case L:
              bPtr = new LBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
              break;
          case J:
              bPtr = new JBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
              break;
          case T:
              bPtr = new TBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
              break;
          case Z:
              bPtr = new ZBlock(COLOR_PAIR(rand() % nBlockTypes + 1));
              break;
      }
      return bPtr;
  }
  Queue RandomBlockChooser::GetStartingQueue() {
      Queue que;
      que.push_back(GetNext());
      que.push_back(GetNext());
      return que;
  }
  //
  // Todo: Implement your RandomBlockChooser here
  //

}
