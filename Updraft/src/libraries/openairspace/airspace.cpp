#include "airspace.h"

OpenAirspace::Airspace::Coordinate OpenAirspace::Airspace::X;
bool OpenAirspace::Airspace::validX;

namespace OpenAirspace {
  Airspace::Airspace(QTextStream* ts) {
    QString text("");

    this->validAN = false;
    this->validAL = false;
    this->validAH = false;
    this->validAT = false;
    this->validTO = false;
    this->validTC = false;
    this->CW      = true;
    this->Wi      = -1;
    this->validDA = false;
    this->validDB = false;
    this->validDP = false;
    this->validG  = false;
    this->Z       = -1;
    this->validSP = false;
    this->validSB = false;
    this->validDY = false;

    (*ts) >> text;
    if (text != "AC") return;

    (*ts) >> text;
    // this->AC = text;
    // NA because more classes than defined available e.g. "E" - not in specs
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
      return;

    while (!(*ts).atEnd()) {
      (*ts) >> text;

      if (text == "AC")
        return;
      if (text.size() == 0 || text.at(0) == '*')
        continue;

      QString parse;
      ts->skipWhiteSpace();
      parse = ts->readLine();

      int comment = parse.indexOf('*');
      if (comment > -1)
        parse = parse.left(comment);

      if (text == "AN") {
        if (this->validAN)
          *this->AN = parse;
        else
          this->AN = new QString(parse);
        qDebug("%s", AN->toAscii().data());
        this->validAN = true;
      } else if (text == "AL") {
        if (this->validAL)
          *this->AL = parse;
        else
          this->AL = new QString(parse);  // can be text e.g. Ask on 122.8
        this->validAL = true;
      } else if (text == "AH") {
        if (this->validAH)
          *this->AH = parse;
        else
          this->AH = new QString(parse);  // can be text e.g. Ask on 122.8
        this->validAH = true;
      } else if (text == "AT") {
        if (!this->validAT)
          this->AT = new QList<Coordinate*>();
        this->AT->push_back(ParseCoord(parse));
        this->validAT = true;
      } else if (text == "TO") {
        if (this->validTO)
          *this->TO = parse;
        else
          this->TO = new QString(parse);
        this->validTO = true;
      } else if (text == "TC") {
        if (this->validTC)
          *this->TC = parse;
        else
          this->TC = new QString(parse);
        this->validTC = true;
      } else if (text == "SP") {
        if (!this->validSP)
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
        this->validSP = true;
      } else if (text == "SB") {
        if (this->validSB)
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
        this->validSB = true;
      } else if (text == "V") {
        QChar ch = parse.at(0);
        parse = parse.right(parse.size() - parse.indexOf('=') -1);
        if (ch == 'X') {
          // this->X = ParseCoord(parse);
          X = *ParseCoord(parse);
          this->validX = true;
        } else if (ch == 'D') {
          if (parse.trimmed().at(0) == '-')
            this->CW = false;
          else
            this->CW = true;
        } else if (ch == 'W') {
          this->Wi = parse.toInt();
        } else if (ch == 'Z') {
          this->Z = parse.toFloat();
          // this->validZ = true;
        }
      }  /* V */      else if (text == "DP") {
        if (!this->validG) {
          // this->geometry = new QList< QPair<GType, void*> >();
          this->geometry = new QList<void*>();
          this->validG = true;
        }
        Poly* p = new Poly();
        p->type = DPtype;
        p->coor = ParseCoord(parse);
        // this->geometry->push_back(QPair<GType, void*>(DPtype, p));
        this->geometry->push_back(p);
      } else if (text == "DA") {
        if (!this->validG) {
          this->geometry = new QList<void*>();
          // this->geometry = new QList< QPair<GType, void*> >();
          this->validG = true;
        }
        ArcI* arc = new ArcI();
        arc->type = DAtype;
        arc->Zoom = this->Z;
        arc->Centre = new Coordinate(X);
        arc->CW = this->CW;
        int i = parse.indexOf(',');
        arc->R = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        arc->Start = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        arc->End = parse.toInt();
        arc->valid = true;
        // this->DA.push_back(arc);
        // this->geometry->push_back(QPair<GType, void*>(DAtype, arc));
        this->geometry->push_back(arc);
      } else if (text == "DB") {
        if (!this->validG) {
          this->geometry = new QList<void*>();
          // this->geometry = new QList< QPair<GType, void*> >();
          this->validG = true;
        }
        ArcII* arc = new ArcII();
        arc->type = DBtype;
        arc->Zoom = this->Z;
        arc->Centre = new Coordinate(X);
        arc->CW = this->CW;
        int i = parse.indexOf(',');
        arc->Start = ParseCoord(parse.left(i));
        parse = parse.right(parse.size() - i -1);
        arc->End = ParseCoord(parse);
        arc->valid = true;
        // this->DB.push_back(arc);
        // this->geometry->push_back(QPair<GType, void*>(DBtype, arc));
        this->geometry->push_back(arc);
      } else if (text == "DC") {
        if (!this->validG) {
          this->geometry = new QList<void*>();
          // this->geometry = new QList< QPair<GType, void*> >();
          this->validG = true;
        }
        Circle* cir = new Circle();
        cir->type = DCtype;
        cir->Zoom = this->Z;
        cir->Centre = new Coordinate(X);
        cir->R = parse.toInt();
        cir->valid = true;
        // this->DC.push_back(cir);
        // this->geometry->push_back(QPair<GType, void*>(DCtype, cir));
        this->geometry->push_back(cir);
      } else if (text == "DY") {
        this->DY->push_back(ParseCoord(parse));
      }
    }
  }

  Airspace::Coordinate* Airspace::ParseCoord(const QString& text) {
    Coordinate* cor = new Coordinate;
    int i = 0, j = 0;
    while (text.at(i) != 'N' && text.at(i) != 'S' && i < text.size()) ++i;
    while (text.at(j) != 'E' && text.at(j) != 'W' && j < text.size()) ++j;
    if (i == text.size() || j == text.size()) {
      qWarning("Error parsing coordinates in Airspace file: N/S/E/W nt found.");
      return cor;
    }
    QString parse = text.left(i);
    int parts = parse.count(':');
    switch (parts) {
      case 0 :
        cor->lat = parse.toDouble();
        if (text.at(i) == 'S') cor->lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor->lon = parse.toDouble();
        if (text.at(j) == 'W') cor->lon *= -1;
        break;
      case 1 :
        cor->lat = parse.section(':', 0, 0).toDouble()
          + parse.section(':', 1, 1).toDouble() / 60;
        if (text.at(i) == 'S') cor->lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor->lon = parse.section(':', 0, 0).toDouble()
          + parse.section(':', 1, 1).toDouble() / 60;
        if (text.at(j) == 'W') cor->lon *= -1;
        break;
      case 2 :
        cor->lat = parse.section(':', 0, 0).toDouble()
          + (parse.section(':', 1, 1).toDouble()
          + parse.section(':', 2, 2).toDouble() / 60) / 60;
        if (text.at(i) == 'S') cor->lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor->lon = parse.section(':', 0, 0).toDouble()
          + (parse.section(':', 1, 1).toDouble()
          + parse.section(':', 2, 2).toDouble() / 60) / 60;
        if (text.at(j) == 'W') cor->lon *= -1;
        break;
      default :
        qWarning("Error parsing coordinates in Airspace file: ':' not found.");
        cor->valid = false;
        return cor;
    }
    cor->valid = true;
    return cor;
  }

  Airspace::~Airspace() {
    if (validAN) {
      delete AN;
      AN = NULL;
    }
    if (validAL) {
      delete AL;
      AL = NULL;
    }
    if (validAH) {
      delete AH;
      AH = NULL;
    }
    if (validAT) {
      for (int i = 0; i < AT->size(); ++i) {
        delete AT->at(i);
      }
      if (AT->size() != 0)
        delete AT;
    }
    if (validTO) {
      delete TO;
      TO = NULL;
    }
    if (validTC) {
      delete TC;
      TC = NULL;
    }
    if (validSP) {
      delete SP;
      SP = NULL;
    }
    if (validSB) {
      delete SB;
      SB = NULL;
    }
    if (this->validG) {
      for (int i = 0; i < geometry->size(); ++i) {
        // delete geometry->at(i).second;
        delete geometry->at(i);
      }
      delete geometry;
      geometry = NULL;
    }

    // DP,DC,DA,DB
  }
}  // OpenAirspace
