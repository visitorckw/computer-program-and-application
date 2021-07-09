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

#include "Ui.hpp"
#include "Config.hpp"
#include "BlockChooser.hpp"
#include <boost/assign.hpp>

//DBG
#include <iostream>
#include <boost/format.hpp>

using namespace Bastet;
using namespace std;
using namespace boost;
using namespace boost::assign;


int main(int argc, char **argv){
  Ui ui;
  while(1){       
    int choice = ui.MenuDialog(list_of("Play - Normal")("Play - Difficult")("View highscores")("Customize keys")("Quit"));
    switch(choice){
    case 0: {
      NoviceBlockChooser bc;
      ui.Play(&bc);
      ui.HandleHighScores(difficulty_normal);
      ui.ShowHighScores(difficulty_normal);
      }
      break;
    case 1: {
        RandomBlockChooser rd;
        ui.Play(&rd);
        ui.HandleHighScores(difficulty_normal);
        ui.ShowHighScores(difficulty_normal);
    }
      //
      // Todo: Create an instance of your RandomBlockChooser here and start the game
      //
      break;
    case 2:
      ui.ShowHighScores(difficulty_normal);
      ui.ShowHighScores(difficulty_hard);
      break;
    case 3:
      ui.CustomizeKeys();
      break;
    case 4:
      exit(0);
      break;
    }
  }
}
