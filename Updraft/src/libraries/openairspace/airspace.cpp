#include "airspace.h"

OpenAirspace::Position OpenAirspace::Airspace::X;
bool OpenAirspace::Airspace::validX;

namespace OpenAirspace {
  Airspace::Airspace(QTextStream* ts) {
    QString text("");

    /* testing :
    Position* pos = new Position();
    pos->lat = 10;
    pos->lon = 20;
    QVector<Geometry*> g;
    Polygon* pp = new Polygon(*pos, 1);
    g.push_back(pp);
    OpenAirspace::ArcI* aa = new OpenAirspace::ArcI(*pos, 10, true,
      30, 40, 1);
    g.push_back(aa);
    Geometry* gg = g.at(0);
    delete gg;
    gg = g.at(1);
    delete gg;
    OpenAirspace::ArcII* ab = new OpenAirspace::ArcII(
      *pos, *pos, *pos, true, 1);
    g.push_back(ab);
    OpenAirspace::Circle* cc = new OpenAirspace::Circle(*pos, 10, 1);
    g.push_back(cc);
    gg = g.at(2);
    delete gg;
    gg = g.at(3);
    delete gg;
    */


    this->AL        = NULL;
    this->AH        = NULL;
    this->AN        = NULL;
    this->DY        = NULL;
    this->SP        = NULL;
    this->SB        = NULL;
    this->TO        = NULL;
    this->TC        = NULL;
    this->AT        = NULL;
    this->geometry  = NULL;
    this->ACstring  = NULL;
    this->CW        = true;
    this->Wi        = -1;
    this->Z         = -1;

    (*ts) >> text;
    if (text != "AC") return;

    (*ts) >> text;
    if (!this->ACstring)
      this->ACstring = new QString(text);
    else
      *this->ACstring = text;
    if (text      == "R") this->AC = R;
    else if (text == "Q") this->AC = Q;
    else if (text == "P") this->AC = P;
    else if (text == "A") this->AC = A;
    else if (text == "B") this->AC = B;
    else if (text == "C") this->AC = C;
    else if (text == "D") this->AC = D;
    else if (text == "E") this->AC = E;
    else if (text == "GP") this->AC = GP;
    else if (text == "CTR") this->AC = CTR;
    else if (text == "W") this->AC = W;
    else
      this->AC = NA;
    // return;

    while (!(*ts).atEnd()) {
      (*ts) >> text;
      // int check = ts->pos();

      if (text == "AC")
        return;
      if (text.size() == 0 || text.at(0) == '*') {
        if (text.size() != 0)
          text = ts->readLine();
        continue;
      }

      QString parse;
      ts->skipWhiteSpace();
      parse = ts->readLine();

      int comment = parse.indexOf('*');
      if (comment > -1)
        parse = parse.left(comment);

      if (text == "AN") {
        if (this->AN)
          *this->AN = parse;
        else
          this->AN = new QString(parse);
        // qDebug("%s", AN->toAscii().data());
      } else if (text == "AL") {
        if (this->AL)
          *this->AL = parse;
        else
          this->AL = new QString(parse);  // can be text e.g. Ask on 122.8
      } else if (text == "AH") {
        if (this->AH)
          *this->AH = parse;
        else
          // can be text e.g. Ask on 122.8
          this->AH = new QString(parse);
      } else if (text == "AT") {
        if (!this->AT)
          this->AT = new QVector<Position*>();
        Position* pos = new Position(ParseCoord(parse));
        this->AT->push_back(pos);
      } else if (text == "TO") {
        if (this->TO)
          *this->TO = parse;
        else
          this->TO = new QString(parse);
      } else if (text == "TC") {
        if (this->TC)
          *this->TC = parse;
        else
          this->TC = new QString(parse);
      } else if (text == "SP") {
        if (!this->SP)
          this->SP = new SP_str();
        int i = parse.indexOf(',');
        this->SP->style = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        this->SP->width = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        this->SP->R = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        this->SP->G = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        this->SP->B = parse.left(i).toInt();
      } else if (text == "SB") {
        if (!this->SB)
          this->SB = new SB_str();
        int i = parse.indexOf(',');
        this->SB->R = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        this->SB->G = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        this->SB->B = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
      } else if (text == "V") {
        QChar ch = parse.at(0);
        parse = parse.right(parse.size() - parse.indexOf('=') -1);
        if (ch == 'X') {
          X = ParseCoord(parse);
          this->validX = true;
        } else if (ch == 'D') {
          if (parse.trimmed().at(0) == '-')
            this->CW = false;
          else
            this->CW = true;
        } else if (ch == 'W') {
          this->Wi = parse.toDouble();
        } else if (ch == 'Z') {
          this->Z = parse.toFloat();
          // this->validZ = true;
        }
      }  /* V */      else if (text == "DP") {
        if (!this->geometry) {
          this->geometry = new QVector<Geometry*>();
        }
        Polygon* p = new Polygon(ParseCoord(parse), this->Z);
        this->geometry->push_back(p);
      } else if (text == "DA") {
        if (!this->geometry)
          this->geometry = new QVector<Geometry*>();
        int i = parse.indexOf(',');
        double R = parse.left(i).toDouble();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        double Start = parse.left(i).toDouble();
        parse = parse.right(parse.size() - i -1);
        double End = parse.toDouble();
        ArcI* arc = new
          ArcI(X, R, this->CW, Start, End, this->Z);
        this->geometry->push_back(arc);
      } else if (text == "DB") {
        if (!this->geometry)
          this->geometry = new QVector<Geometry*>();
        int i = parse.indexOf(',');
        Position Start = ParseCoord(parse.left(i));
        parse = parse.right(parse.size() - i -1);
        Position End = ParseCoord(parse);
        ArcII* arc = new
          ArcII(X, Start, End, this->CW, this->Z);
        this->geometry->push_back(arc);
      } else if (text == "DC") {
        if (!this->geometry)
          this->geometry = new QVector<Geometry*>();
        Circle* cir = new Circle(X, parse.toDouble(), this->Z);
        this->geometry->push_back(cir);
      } else if (text == "DY") {
        Position* pos = new Position(ParseCoord(parse));
        if (!this->DY)
          this->DY = new QVector<Position*>;
        this->DY->push_back(pos);
      }
    }
  }

  /*Airspace::Airspace(const Airspace& par)
  {
    this->AN = par.AN;
    return *this;
  }*/

  Position Airspace::ParseCoord(const QString& text) {
    Position cor;
    int i = 0, j = 0;
    while (i < text.size() && text.at(i) != 'N' && text.at(i) != 'S'
      && text.at(i) != 'n' && text.at(i) != 's') ++i;
    while (j < text.size() && text.at(j) != 'E' && text.at(j) != 'W'
      && text.at(j) != 'e' && text.at(j) != 'w') ++j;
    if (i == text.size() || j == text.size()) {
      qWarning("Error parsing coords in Airspace file: N/S/E/W not found.");
      cor.valid = false;
      return cor;
    }
    QString parse = text.left(i);
    int parts = parse.count(':');
    switch (parts) {
      case 0 :
        cor.lat = parse.toDouble();
        if (text.at(i) == 'S' || text.at(i) == 's') cor.lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor.lon = parse.toDouble();
        if (text.at(j) == 'W' || text.at(j) == 'w') cor.lon *= -1;
        break;
      case 1 :
        cor.lat = parse.section(':', 0, 0).toDouble()
          + parse.section(':', 1, 1).toDouble() / 60;
        if (text.at(i) == 'S' || text.at(i) == 's') cor.lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor.lon = parse.section(':', 0, 0).toDouble()
          + parse.section(':', 1, 1).toDouble() / 60;
        if (text.at(j) == 'W' || text.at(j) == 'w') cor.lon *= -1;
        break;
      case 2 :
        cor.lat = parse.section(':', 0, 0).toDouble()
          + (parse.section(':', 1, 1).toDouble()
          + parse.section(':', 2, 2).toDouble() / 60) / 60;
        if (text.at(i) == 'S' || text.at(i) == 's') cor.lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor.lon = parse.section(':', 0, 0).toDouble()
          + (parse.section(':', 1, 1).toDouble()
          + parse.section(':', 2, 2).toDouble() / 60) / 60;
        if (text.at(j) == 'W' || text.at(j) == 'w') cor.lon *= -1;
        break;
      default :
        qWarning("Error parsing coordinates in Airspace file: ':' not found.");
        cor.valid = false;
        return cor;
    }
    cor.valid = true;
    return cor;
  }

  Airspace::~Airspace() {
    if (AN) {
      delete AN;
      AN = NULL;
    }
    if (AL) {
      delete AL;
      AL = NULL;
    }
    if (AL) {
      delete AH;
      AH = NULL;
    }
    if (AT) {
      for (int i = 0; i < AT->size(); ++i) {
        delete AT->at(i);
      }
      if (AT->size() != 0)
        delete AT;
    }
    if (TO) {
      delete TO;
      TO = NULL;
    }
    if (TC) {
      delete TC;
      TC = NULL;
    }
    if (SP) {
      delete SP;
      SP = NULL;
    }
    if (SB) {
      delete SB;
      SB = NULL;
    }
    if (geometry) {
      for (int i = 0; i < geometry->size(); ++i) {
        Geometry* g = this->geometry->at(i);
        delete g;
      }
      delete geometry;
      geometry = NULL;
    }
    if (DY) {
      for (int i = 0; i < this->DY->size(); i++)
        delete this->DY->at(i);
      delete this->DY;
      this->DY = NULL;
    }
    if (this->ACstring) {
      delete this->ACstring;
      this->ACstring = NULL;
    }
  }

int Airspace::ParseHeight(bool floor, bool* agl) {
  // parse the string to number in ft
  // if none of cond hit, return 0
  int absoluteHeightInFt = 0;
  *agl = false;
  QString* parsedString;

  // set the related parsed text
  if (floor) {
    if (this->AL)
      parsedString = this->AL;
    else
      return 0;
  } else {
    if (this->AH)
      parsedString = this->AH;
    else
      return 0;
  }

  if (parsedString->contains("FL", Qt::CaseInsensitive)) {
    // Compute the flight level = QNH1013.25 hPa MSL
    int s = parsedString->indexOf(QRegExp("[0-9]"));
    int e = parsedString->lastIndexOf(QRegExp("[0-9]"));
    int FL = parsedString->mid(s, e - s + 1).toInt();
    absoluteHeightInFt = FL * 100;
  } else if (parsedString->contains("MSL", Qt::CaseInsensitive)) {
    int s = parsedString->indexOf(QRegExp("[0-9]"));
    int e = parsedString->lastIndexOf(QRegExp("[0-9]"));
    absoluteHeightInFt = parsedString->mid(s, e - s + 1).toInt();
  } else if (parsedString->contains("AGL", Qt::CaseInsensitive)) {
    int s = parsedString->indexOf(QRegExp("[0-9]"));
    int e = parsedString->lastIndexOf(QRegExp("[0-9]"));
    absoluteHeightInFt = parsedString->mid(s, e - s + 1).toInt();
    *agl = true;
  }

  return absoluteHeightInFt;
}
}  // OpenAirspace
