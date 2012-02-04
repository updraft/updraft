#include "airspace.h"

OpenAirspace::Airspace::Coordinate OpenAirspace::Airspace::X;

namespace OpenAirspace {
  Airspace::Airspace(QTextStream* ts) {
    QString text("");

    // X.valid = false;

    this->validAN = false;
    this->validAL = false;
    this->validAH = false;
    this->validTO = false;
    this->validTC = false;
    // this->validX  = false;
    this->CW      = true;
    this->Wi      = -1;
    // this->validZ  = false;
    this->validDA = false;
    this->validDB = false;
    // float Airspace::Z;
    // this->X.valid = false;
    this->Z = -1;
    this->SP.valid = false;
    this->SB.valid = false;

    (*ts) >> text;
    if (text != "AC") return;

    (*ts) >> text;
    this->AC = text;
    /* NA because more classes than defined available e.g. "E" - not in specs
    if (text      == "R") this->AC = R;
    else if (text == "Q") this->AC = Q;
    else if (text == "P") this->AC = P;
    else if (text == "A") this->AC = A;
    else if (text == "B") this->AC = B;
    else if (text == "C") this->AC = C;
    else if (text == "D") this->AC = D;
    else if (text == "GP") this->AC = GP;
    else if (text == "CTR") this->AC = CTR;
    else if (text == "W") this->AC = W;
    else
      return;*/

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
        if (validAN)
          *this->AN = parse;
        else
          this->AN = new QString(parse);
        qDebug("%s", AN.toAscii().data());
        validAN = true;
      } else if (text == "AL") {
        if (validAL)
          *this->validAL = parse;
        else
          this->AL = new QString(parse);  // !!! can be text e.g. Ask on 122.8 !!!
        validAL = true;
      } else if (text == "AH") {
        if (validAH)
          *this->AH = parse;
        else
          this->AH = new QString(parse);  // !!! can be text e.g. Ask on 122.8 !!!
        validAH = true;
      } else if (text == "AT") {
        if (!validAT)
          this->AT = new QList;
        this->AT->push_back(ParseCoord(parse));
        validAT = true;
      } else if (text == "TO") {
        if (validTO)
          *this->TO = parse;
        else
          this->TO = new QString(parse);
        validTO = true;
      } else if (text == "TC") {
        if (validTC)
          *this->TC = parse;
        else
          this->TC = new QString(parse);
        validTC = true;
      } else if (text == "SP") {
        int i = parse.indexOf(',');
        SP.style = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.width = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.R = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.G = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.B = parse.left(i).toInt();
        SP.valid = (SP.B < 0 || SP.G < 0 || SP.R < 0) ?
          false : true;
      } else if (text == "SB") {
        int i = parse.indexOf(',');
        SB.R = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SB.G = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SB.B = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        SB.valid = (SB.B < 0 || SB.G < 0 || SB.R < 0) ?
          false : true;
      } else if (text == "V") {
        QChar ch = parse.at(0);
        parse = parse.right(parse.size() - parse.indexOf('=') -1);
        if (ch == 'X') {
          // this->X = ParseCoord(parse);
          X = ParseCoord(parse);
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
        this->DP.push_back(ParseCoord(parse));
      } else if (text == "DA") {
        // init
        ArcI arc;
        /*if (validZ)*/ arc.Zoom = this->Z;
        /*if (validX)*/ arc.Centre = X;
        arc.CW = this->CW;

        // parse row
        int i = parse.indexOf(',');
        arc.R = parse.left(i).toInt();

        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        arc.Start = parse.left(i).toInt();

        parse = parse.right(parse.size() - i -1);
        arc.End = parse.toInt();

        arc.valid = true;
        this->DA.push_back(arc);
      } else if (text == "DB") {
        ArcII arc;
        /*if (validZ)*/ arc.Zoom = this->Z;
        /*if (validX)*/ arc.Centre = X;
        arc.CW = this->CW;

        int i = parse.indexOf(',');
        arc.Start = ParseCoord(parse.left(i));

        parse = parse.right(parse.size() - i -1);
        arc.End = arc.Start = ParseCoord(parse);

        arc.valid = true;
        this->DB.push_back(arc);
      } else if (text == "DC") {
        Circle cir;
        /*if (validZ)*/ cir.Zoom = this->Z;
        /*if (validX)*/ cir.Centre = X;

        // parse row
        cir.R = parse.toInt();
        cir.valid = true;
        this->DC.push_back(cir);
      } else if (text == "DY") {
        this->DY.push_back(ParseCoord(parse));
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
      return Coordinate();
    }
    QString parse = text.left(i);
    int parts = parse.count(':');
    switch (parts) {
      case 0 :
        cor->lat = parse.toDouble();
        if (text.at(i) == 'S') cor.lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor->lon = parse.toDouble();
        if (text.at(j) == 'W') cor.lon *= -1;
        break;
      case 1 :
        cor->lat = parse.section(':', 0, 0).toDouble()
          + parse.section(':', 1, 1).toDouble() / 60;
        if (text.at(i) == 'S') cor.lat *= -1;
        parse = text.mid(i+1, j - i - 1);
        cor->lon = parse.section(':', 0, 0).toDouble()
          + parse.section(':', 1, 1).toDouble() / 60;
        if (text.at(j) == 'W') cor.lon *= -1;
        break;
      case 2 :
        cor->lat = parse.section(':', 0, 0).toDouble()
          + (parse.section(':', 1, 1).toDouble()
          + parse.section(':', 2, 2).toDouble() / 60) / 60;
        if (text.at(i) == 'S') cor.lat *= -1;
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
      AN = null;
    }
    if (validAL) {
      delete AL;
      AL = null;
    }
    if (validAH) {
      delete AH;
      AH = null;
    }
    for (size_t i = 0; i < AT->size(); ++i) {
      delete AT->at(i);
      AT->at(i) = null;
    }
    if (AT->size() != 0)
      delete AT;
    if (validTO) {
      delete TO;
      TO = null;
    }
    if (validTC) {
      delete TC;
      TC = null;
    }
    if (validSP) {
      delete SP;
      SP = null;
    }
    if (validSB) {
      delete SB;
      SB = null;
    }
    for (size_t i = 0; i < geometry-size(); ++i)
    {
      delete geometry->at(i);
      geometry->at(i) = null;
    }
    if (geometry->size() != 0)
      delete geometry;
  }
}  // OpenAirspace
