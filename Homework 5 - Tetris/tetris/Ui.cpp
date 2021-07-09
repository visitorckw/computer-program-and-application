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

#include <cstdio>
#include <cstdlib>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "curses.h"

using namespace std;
using namespace boost;

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void msleep(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

namespace Bastet{

  Score &operator +=(Score &a, const Score &b){
    a.first+=b.first;
    a.second+=b.second;
    return a;
  }

  void voidendwin(){
    endwin();
  }

  void PrepareUiGetch(){ ///gets ready for a getch() in the UI, i.e. empties the char buffer, sets blocking IO
    nodelay(stdscr,TRUE);
    while(getch()!=ERR);
    nodelay(stdscr,FALSE);
  }
  
  BorderedWindow::BorderedWindow(int height, int width, int y, int x){
    if(y==-1 || x==-1){
      int screen_h, screen_w;
      getmaxyx(stdscr,screen_h, screen_w);
      if(y==-1)
	    y=(screen_h-height-2)/2-1;
      if(x==-1)
	    x=(screen_w-width-2)/2-1;
    }
    _border = newwin(height+2, width+2, y, x);
    _window = derwin(_border, height, width, 1, 1); 
    RedrawBorder();
  }

  BorderedWindow::~BorderedWindow(){
    delwin(_window);
    delwin(_border);
  }
  
  BorderedWindow::operator WINDOW*(){
    return _window;
  }

  void BorderedWindow::RedrawBorder(){
    //box(_border, 0, 0);
    box(_border, '|', '-'); // RSLiu
    wrefresh(_border);
  }
 
  int BorderedWindow::GetMinX(){
    int x, y;
    getbegyx(_border,y,x);
    (void)(y); //silence warning about unused y
    return x;
  }

  int BorderedWindow::GetMinY(){
    int y, x;
    getbegyx(_border,y,x);
    return y;
  }

  int BorderedWindow::GetMaxX(){
    int x, y;
    getmaxyx(_border, y, x);
    (void)(y); //silence warning about unused y
    return GetMinX()+x;
  }

  int BorderedWindow::GetMaxY(){
    int y,x;
    getmaxyx(_border, y, x);
    return GetMinY()+y;
  }

  void BorderedWindow::DrawDot(const Dot &d, Color c){
    wattrset((WINDOW *)(*this), c);
    mvwaddch(*this,d.y, 2*d.x, ' ');
    mvwaddch(*this,d.y, 2*d.x+1, ' ');
  }

  Curses::Curses(){    
    if(initscr()==NULL){
      fprintf(stderr,"bastet: error while initializing graphics (ncurses library).\n");
      exit(1);
    }
    if(!has_colors()){
      endwin();
      fprintf(stderr,"bastet: no color support, sorry. Ask the author for a black and white version.");
      exit(1);
    }
    
    /* Turn off cursor. */
    curs_set(0);
    atexit(voidendwin); /*make sure curses are properly stopped*/
    
    /* Setup keyboard. We'd like to get each and every character, but
       not to display them on the terminal. */
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    nonl();
    noecho();
    cbreak();
    setlocale(LC_ALL, ""); //rsliu
    
    start_color();
    /* 1 - 16 is for blocks */
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_BLUE);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    
    
    /* 17 - ? is for other things */
    init_pair(17, COLOR_RED,    COLOR_BLACK); //points
    init_pair(18, COLOR_YELLOW, COLOR_BLACK); //number of lines
    init_pair(19, COLOR_GREEN,  COLOR_BLACK); //level
    init_pair(20, COLOR_YELLOW, COLOR_BLACK); //messages
    init_pair(21, COLOR_WHITE,  COLOR_BLACK); //window borders
    init_pair(22, COLOR_WHITE,  COLOR_BLACK); //end of line animation

    /* Set random seed. */
    srand(time(NULL)+37);
  }

  Ui::Ui():
    _level(0),
    _wellWin(WellHeight,2*WellWidth),
    _nextWin(5,14,_wellWin.GetMinY(),_wellWin.GetMaxX()+1),
    _scoreWin(7,14,_nextWin.GetMaxY(),_nextWin.GetMinX())
  {
    BOOST_FOREACH(ColorWellLine &a, _colors)
      a.assign(0);
  }

  Dot BoundingRect(const std::string &message){ //returns x and y of the minimal rectangle containing the given string
    vector<string> splits;
    split(splits,message,is_any_of("\n"));
    size_t maxlen=0;
    BOOST_FOREACH(string &s, splits){
      maxlen=max(maxlen,s.size());
    }
    Dot r = { int(maxlen + 1),int(splits.size()) };
    return r;
  }

  void Ui::MessageDialog(const std::string &message){
    RedrawStatic();

    Dot d=BoundingRect(message);

    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    int ch;
    do{
      ch=getch();
    }
    while(ch!=' ' && ch!=13); //13=return key!=KEY_ENTER, it seems
  }

  std::string Ui::InputDialog(const std::string &message){
    RedrawStatic();
    Dot d=BoundingRect(message);
    d.y+=3;
    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    //nocbreak();
    echo();
    curs_set(1);
    char buf[51];
    mvwgetnstr(w,d.y-2,1,buf,50);
    curs_set(0);
    noecho();
    return string(buf);
  }
  
  int Ui::KeyDialog(const std::string &message){
    RedrawStatic();

    Dot d=BoundingRect(message);

    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    return getch();
  }
  
  int Ui::MenuDialog(const vector<string> &choices){
    RedrawStatic();
    size_t width=0;
    BOOST_FOREACH(const string &s, choices){
      width=max(width,s.size());
    }

    Dot d={int(width+5),int(choices.size())};
    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    for(size_t i=0;i<choices.size();++i){
      mvwprintw(w,i,4,choices[i].c_str());
    }
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    size_t chosen=0;
    int ch;
    bool done=false;
    mvwprintw(w,chosen,1,"-> ");
    wrefresh(w);
    do{
      ch=getch();
      switch(ch){
      case KEY_UP:
	if(chosen==0) break;
	mvwprintw(w,chosen,1,"   ");
	chosen--;
	mvwprintw(w,chosen,1,"-> ");
	wrefresh(w);
	break;
      case KEY_DOWN:
	if(chosen==choices.size()-1) break;
	mvwprintw(w,chosen,1,"   ");
	chosen++;
	mvwprintw(w,chosen,1,"-> ");
	wrefresh(w);
	break;
      case 13: //ENTER
      case ' ':
	done=true;
	break;
      }
    } while(!done);
    return chosen;
  }
  
  void Ui::ChooseLevel(){
    RedrawStatic();
    int ch='0';
    format fmt("    Get ready!\n"
	       " \n"
	       " Starting level = %1% \n"
	       " 0-9 to change\n"
	       " <SPACE> to start\n");
    string msg;
    while(ch!=' '){
      msg=str(fmt % _level);
      PrepareUiGetch();
      Dot d = BoundingRect(msg );
      BorderedWindow w(d.y, d.x);
      wattrset((WINDOW *)w,COLOR_PAIR(20));
      mvwprintw(w, 0, 0, msg.c_str());
      w.RedrawBorder();
      ch = getch();
      switch(ch){
      case '0': 
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
	    _level = ch-'0';
      }
    }
    assert(_level>=0 && _level<=9);
  }

  void Ui::RedrawStatic(){
    erase();
    wrefresh(stdscr);
    _wellWin.RedrawBorder();
    _nextWin.RedrawBorder();
    _scoreWin.RedrawBorder();

    wattrset((WINDOW*)_nextWin,COLOR_PAIR(17));
    mvwprintw(_nextWin,0,0," Next block:");
    wrefresh(_nextWin);

    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(17));
    mvwprintw(_scoreWin, 1, 0, "Score:");
    wattrset((WINDOW*)_scoreWin, COLOR_PAIR(18));
    mvwprintw(_scoreWin, 3, 0, "Lines:");
    wattrset((WINDOW*)_scoreWin, COLOR_PAIR(19));
    mvwprintw(_scoreWin, 5, 0, "Level:");
    wrefresh(_scoreWin);
  }

  //must be <1E+06, because it should fit into a timeval usec field(see man select)
  static const boost::array<int,10> delay = {{999999, 770000, 593000, 457000, 352000, 271000, 208000, 160000, 124000, 95000}};

  void Ui::DropBlock(Block* b, Well *w){

    int delay_msec = delay[_level] / 2000;
    timeout(delay_msec);
    keypad(stdscr, TRUE);
    
    //assumes nodelay(stdscr,TRUE) has already been called

    RedrawWell(w, b);
    Keys *keys = config.GetKeys();

    while(1) { //break = tetromino locked        
        int ch = getch();
	    if(ch == keys->Left)
	        b->MoveIfPossible(Left, w);
	    else if(ch == keys->Right)
	        b->MoveIfPossible(Right, w);
        else if (ch == keys->Down) {
            bool val = b->MoveIfPossible(Down, w);
            if (val) {
                delay_msec = delay[_level] / 2000;
            }
            else break;
        }
	    else if(ch == keys->RotateCW)
	        b->MoveIfPossible(RotateCW, w);
	    else if(ch == keys->RotateCCW)
	        b->MoveIfPossible(RotateCCW, w);
        else if (ch == keys->Drop) {
            b->Drop(w);
            break;
        }
	    else if(ch == keys->Pause){
	        MessageDialog("Press SPACE or ENTER to resume the game");
	        RedrawStatic();
	        RedrawWell(w, b);
	        nodelay(stdscr,TRUE);
	    }
	    else {
            if (!b->MoveIfPossible(Down, w))
                break;
        } //timeout...    
        RedrawWell(w, b);
    } //while(1) 

    // Get completed lines
    LinesCompleted lc = w->Lock(b);

    // Locks also into _colors
    BOOST_FOREACH(const Dot & d, b->GetDotsInWell()) {
        if (d.y >= 0)
            _colors[d.y + 2][d.x] = b->GetColor();
    }

    RedrawWell(w, b);
    if(lc._completed.any()){
      CompletedLinesAnimation(lc);
      w->ClearLines(lc);
      //clears also _colors
      ColorWell::reverse_iterator it=lc.Clear(_colors.rbegin(),_colors.rend());
      for(; it!=_colors.rend(); ++it){
	    it->assign(0);
      }
      
      int newlines=lc._completed.count();
      if(( (_lines+newlines)/10 - _lines/10 !=0) && _level<9) {
	    _level++;
      }

      _lines+=newlines;
      switch(newlines){
      case 1:
	    _points+=100;
	break;
      case 2:
	    _points+=300;
	break;
      case 3:
	    _points+=500;
	break;
      case 4:
	    _points+=800;
	break;
      }
      RedrawScore();
    }
  }

  void Ui::RedrawWell(const Well *w, const Block* b){
    for(int i = 0; i < WellWidth; ++i)
      for(int j = 0; j < WellHeight; ++j){
	    Dot d = {i,j};
	    _wellWin.DrawDot(d, _colors[j+2][i]);
      }
    
    BOOST_FOREACH(const Dot &d, b->GetDotsInWell())
      _wellWin.DrawDot(d, b->GetColor());

    wrefresh(_wellWin);
  }

  void Ui::ClearNext(){
    wmove((WINDOW*)_nextWin,1,0);
    wclrtobot((WINDOW*)_nextWin);
    wrefresh(_nextWin);
  }

  void Ui::RedrawNext(const Block* b){
    wmove((WINDOW*)_nextWin,1,0);
    wclrtobot((WINDOW*)_nextWin);

    BOOST_FOREACH(const Dot & d, b->GetDotsRelativeTo({2, 2}))
      _nextWin.DrawDot(d, b->GetColor());
    wrefresh(_nextWin);
  }

  void Ui::RedrawScore(){
    wattrset((WINDOW*)_scoreWin, COLOR_PAIR(17));
    mvwprintw(_scoreWin, 1, 7, "%6d", _points);
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(18));
    mvwprintw(_scoreWin, 3, 7, "%6d", _lines);
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(19));
    mvwprintw(_scoreWin, 5, 7, "%6d", _level);
    wrefresh(_scoreWin);
    return;
  }

  void Ui::CompletedLinesAnimation(const LinesCompleted &completed){
    wattrset((WINDOW*)_wellWin,COLOR_PAIR(22));
    for(int i=0;i<6;++i){
      for(int k=0;k<4;++k){
	    if(completed._completed[k]){
	      wmove(_wellWin,completed._baseY+k,0);
	      whline(_wellWin, i%2?' ':':',WellWidth*2);
	    }
	    wrefresh(_wellWin);
	    //usleep(500000/6);
        msleep(40);
      }
    }
  }

  void Ui::Play(BlockChooser *bc){
    _level=0;
    _points=0;
    _lines=0;
    BOOST_FOREACH(ColorWellLine &a, _colors)
      a.assign(0);
    RedrawStatic();
    RedrawScore();
    Well w;
    nodelay(stdscr,TRUE);

    Queue q = bc->GetStartingQueue();

    if(q.size() == 1) //no block preview for the first block
      ClearNext();

    try{
      while(true){
	    while(getch()!=ERR); //ignores the keys pressed during the next block calculation
	    Block* current = q.front();
	    q.pop_front();
	    if(!q.empty()) RedrawNext(q.front());
	    DropBlock(current, &w);
	    q.push_back(bc->GetNext());
        delete current; // RSLiu
      }
    } catch(GameOver &go){

    }
    return;
  }

  void Ui::HandleHighScores(difficulty_t diff){
    HighScores *hs=config.GetHighScores(diff);
    if(hs->Qualifies(_points)){
      string name=InputDialog(" Congratulations! You got a high score \n Please enter your name");
      hs->InsertHighScore(_points,name);
    }else{
      MessageDialog("You did not get into\n"
		    "the high score list!\n"
		    "\n"
		    "     Try again!\n"
		    );
    }
  }

  void Ui::ShowHighScores(difficulty_t diff){
    HighScores *hs=config.GetHighScores(diff);
    string allscores;
    if(diff==difficulty_normal)
      allscores+="**Normal difficulty**\n";
    else if(diff==difficulty_hard)
      allscores+="**Hard difficulty**\n";
    format fmt("%-20.20s %8d\n");
    for(HighScores::reverse_iterator it=hs->rbegin();it!=hs->rend();++it){
      allscores+=str(fmt % it->Scorer % it->Score);
    }
    MessageDialog(allscores);
  }

  void Ui::CustomizeKeys(){
    Keys *keys=config.GetKeys();
    format fmt(
      "Press the key you wish to use for:\n\n"
      "%=1.34s\n\n");
    keys->Down=KeyDialog(str(fmt % "move tetromino DOWN (soft-drop)"));
    keys->Left=KeyDialog(str(fmt % "move tetromino LEFT"));
    keys->Right=KeyDialog(str(fmt % "move tetromino RIGHT"));
    keys->RotateCW=KeyDialog(str(fmt % "rotate tetromino CLOCKWISE"));
    keys->RotateCCW=KeyDialog(str(fmt % "rotate tetromino COUNTERCLOCKWISE"));
    keys->Drop=KeyDialog(str(fmt % "DROP tetromino (move down as much as possible immediately)"));
    keys->Pause=KeyDialog(str(fmt % "PAUSE the game"));
  }
}
