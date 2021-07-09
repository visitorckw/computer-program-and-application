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

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <boost/array.hpp>
#include "curses.h"

namespace Bastet{

  class Well;

  static const int WellHeight = 20;
  static const int WellWidth = 10;
  static const int RealWellHeight = WellHeight + 2;
  
  typedef int Color; //to be given to wattrset
  
  class Orientation{
  public:
    Orientation(unsigned char o = 0):_o(o){}
    operator unsigned char() const{
      return _o;
    }
    Orientation& operator++(){
        _o = (_o + 1) & 3;
      return *this;
    }
    Orientation& operator--(){
        _o = (_o - 1) & 3;
      return *this;
    }
    const static size_t Number = 4;
  private:
    unsigned char _o;
  };

  enum BlockType{
    O=0,
    I=1,
    Z=2,
    T=3,
    J=4,
    S=5,
    L=6
  };

  const size_t nBlockTypes = 7;
  const size_t nBlockDimension = 4;

  struct Dot;

  typedef boost::array<Dot, nBlockDimension> DotMatrix; //the four dots occupied by a tetromino
  typedef boost::array<DotMatrix, nBlockDimension> OrientationMatrix; //the four orientations of a tetromino

  struct Dot{
    int x;
    int y;
    bool IsValid() const{
      return (y>=-2) && y<WellHeight && (x>=0) && (x<WellWidth);
    }

    Dot operator +(const Dot &d) const{
        Dot r = { x + d.x, y + d.y };
        return r;
    }

    Dot &operator +=(const Dot &d){
      x += d.x; 
      y += d.y;
      return *this;
    }

    DotMatrix operator +(const DotMatrix &b) const{
      DotMatrix m = { {*this + b[0], *this + b[1], *this + b[2], *this + b[3]} };
      return m;
    }

    bool operator==(const Dot &other) const{
      return (x==other.x) && (y==other.y);
    }

    bool operator<(const Dot &other) const{
      if(x==other.x) return y<other.y;
      else return x<other.x;
    }

    //friend size_t hash_value(const Dot &d); //for use with boost::hash and unordered_set
  };

  enum Movement { RotateCW, RotateCCW, Left, Right, Down };

  class Block {
  public:   
      Block(Color c, Dot p = { 3,-2 });      
      bool MoveIfPossible(Movement m, const Well* w);
      void Drop(const Well* w);    
      bool IsOutOfScreen() const;
      const DotMatrix GetDotsInWell() const;  // Get dot positions in the well
      const DotMatrix GetDotsRelativeTo(const Dot& origin) const;   // Get dot positions relative to the origin
      const Dot GetPosition() const;          // Get the current position of the block
      void SetPosition(const Dot);            // Get the current position of the block
      const Color GetColor() const;
      int GetBaseY() const { return _pos.y; } // returns an y s.t. the block lies completely in [y, y+3]

  private:
      // Utility functions
      virtual const OrientationMatrix GetOrientationMatrix() const = 0;
      void Move(Movement m);        
      bool IsValid(const Well* w) const;

      // State variables
      Dot _pos;                                 // Coordinate of the top left corner of the DotMatrix
      Orientation _orientation;                 // Orientation of the block
      Color _color;                             // Block color
  };

  class OBlock : public Block {
  public:
      OBlock(Color c, Dot p = { 3,-2 }): Block(c, p) {};
  private:
      virtual const OrientationMatrix GetOrientationMatrix() const {
          return { {
              {{//orientation 0
                  {1,1},{2,1},{1,0},{2,0}
              }},
              {{//orientation 1
                  {1,1},{2,1},{1,0},{2,0}
              }},
              {{//orientation 2
                  {1,1},{2,1},{1,0},{2,0}
              }},
              {{//orientation 3
                  {1,1},{2,1},{1,0},{2,0}
              }}
            }};
      };
  };

  class IBlock : public Block {
  public:
      IBlock(Color c, Dot p = { 3,-2 }) : Block(c, p) {};
  private:
      virtual const OrientationMatrix GetOrientationMatrix() const {
          return {{ 
            {{//orientation 0 (initial)
                {0,1},{1,1},{2,1},{3,1}
            }},
            {{//orientation 1
                {2,3},{2,1},{2,2},{2,0}
            }},
            {{//orientation 2
                {0,2},{1,2},{2,2},{3,2}
            }},
            {{//orientation 3
                {1,3},{1,1},{1,2},{1,0}
            }}
          }};
      };
  };

  class ZBlock : public Block {
  public:
      ZBlock(Color c, Dot p = { 3,-2 }) : Block(c, p) {};
  private:
      virtual const OrientationMatrix GetOrientationMatrix() const {
          return { {
            {{//orientation 0 (initial)
                {0,1},{1,1},{1,0},{2,0}
            }},
            {{//orientation 1
                {0,0},{0,1},{1,1},{1,2}
            }},
            {{//orientation 2
                {0,2},{1,2},{1,1},{2,1}
            }},
            {{//orientation 3
                {1,0},{1,1},{2,1},{2,2}
            }}
          } };
      };
  };
  class TBlock : public Block {
  public:
      TBlock(Color c, Dot p = { 3,-2 }) : Block(c, p) {};
  private:
      virtual const OrientationMatrix GetOrientationMatrix() const {
          return { {
            {{//orientation 0 (initial)
                {0,1},{1,1},{1,0},{1,2}
            }},
            {{//orientation 1
                {0,1},{1,1},{2,1},{1,2}
            }},
            {{//orientation 2
                {1,0},{1,1},{1,2},{2,1}
            }},
            {{//orientation 3
                {0,1},{1,1},{2,1},{1,0}
            }}
          } };
      };
  };
  class JBlock : public Block {
  public:
      JBlock(Color c, Dot p = { 3,-2 }) : Block(c, p) {};
  private:
      virtual const OrientationMatrix GetOrientationMatrix() const {
          return { {
            {{//orientation 0 (initial)
                {0,1},{0,2},{1,1},{2,1}
            }},
            {{//orientation 1
                {1,0},{1,1},{1,2},{2,2}
            }},
            {{//orientation 2
                {0,1},{1,1},{2,1},{2,0}
            }},
            {{//orientation 3
                {0,0},{1,0},{1,1},{1,2}
            }}
          } };
      };
  };
  class SBlock : public Block {
  public:
      SBlock(Color c, Dot p = { 3,-2 }) : Block(c, p) {};
  private:
      virtual const OrientationMatrix GetOrientationMatrix() const {
          return { {
            {{//orientation 0 (initial)
                {1,1},{1,2},{2,1},{2,2}
            }},
            {{//orientation 1
                {1,1},{1,2},{2,1},{2,2}
            }},
            {{//orientation 2
                {1,1},{1,2},{2,1},{2,2}
            }},
            {{//orientation 3
                {1,1},{1,2},{2,1},{2,2}
            }}
          } };
      };
  };
  class LBlock : public Block {
  public:
      LBlock(Color c, Dot p = { 3,-2 }) : Block(c, p) {};
  private:
      virtual const OrientationMatrix GetOrientationMatrix() const {
          return { {
            {{//orientation 0 (initial)
                {0,0},{0,1},{1,1},{2,1}
            }},
            {{//orientation 1
                {1,0},{1,1},{1,2},{0,2}
            }},
            {{//orientation 2
                {0,1},{1,1},{2,1},{2,2}
            }},
            {{//orientation 3
                {1,0},{1,1},{1,2},{2,0}
            }}
          } };
      };
  };
  //
  // Todo: Define and implement your ZBlock, TBlock, JBlock, SBlock, LBlock classes here
  //
}

#endif //BLOCK_HPP
