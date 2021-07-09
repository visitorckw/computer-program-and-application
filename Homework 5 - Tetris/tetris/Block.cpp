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
#include "Well.hpp"
#include "boost/foreach.hpp"
#include "curses.h"

namespace Bastet{	

	Block::Block(Color c, Dot p) :_color(c), _pos(p) {
	};

	const DotMatrix Block::GetDotsInWell() const {
		return _pos + GetOrientationMatrix()[_orientation];
	}

	const DotMatrix Block::GetDotsRelativeTo(const Dot& origin) const {
		return origin + GetOrientationMatrix()[_orientation];
	}

	const Dot Block::GetPosition() const {
		return _pos;
	}

	void Block::SetPosition(const Dot d) {
		_pos = d;
	}

	const Color Block::GetColor() const {
		return _color;
	}

	void Block::Move(Movement m) {
		switch (m) {
		case RotateCW:
			// Rotate clockwise
			++_orientation;
			break;
		case RotateCCW:
			// Rotate counter clockwise
			--_orientation;
			break;
		case Left:
			_pos.x -= 1;
			break;
		case Right:
			_pos.x += 1;
			break;
		case Down:
			_pos.y += 1;
			break;
		}
	}

	bool Block::MoveIfPossible(Movement m, const Well* w) {
		Dot p = _pos;
		Orientation o = _orientation;
		
		Move(m);

		if (IsValid(w)) {			
			return true;
		}
		else {
			_pos = p;
			_orientation = o;
		}
		
		return false;
	}

	bool Block::IsValid(const Well* w) const {
		return w->Accomodates(GetDotsInWell()); //XX: must change, unoptimized
	}

	void Block::Drop(const Well* w) {
		while (MoveIfPossible(Down, w));
	}

	bool Block::IsOutOfScreen() const {
		BOOST_FOREACH(const Dot & d, GetDotsInWell()) {
			if (d.y >= 0) return false;
		}
		return true;
	}
}
