#include "Kreuzung.h"
#include "Fahrzeug.h"


Kreuzung::Kreuzung(string sName, double dTankstelle):AktivesVO(sName)
{
	p_dTankstelle = dTankstelle;
}


Kreuzung::~Kreuzung()
{
	list<Weg*>::iterator itL;

	for (itL = p_pWege.begin(); itL != p_pWege.end(); itL++)
	{
		delete (*itL);
	}

	cout << "Kreuzung '" << p_sName << "' wurde geloescht" << endl << endl;
}

//Funktion zum Verbinden zweier Kreuzungen. Strassen in Relation zur zu verbindenden Kreuzung
void Kreuzung::vVerbinde(string sWegHin, string sWegZurueck, double dLaenge, Kreuzung* pKreuzung,
	Begrenzung enumBegrenzung, bool bUeberholverbot)
{
	//Wege initialisieren
	Weg* WegHin = new Weg(sWegHin, dLaenge, enumBegrenzung, bUeberholverbot);
	Weg* WegRueck = new Weg(sWegZurueck, dLaenge, enumBegrenzung, bUeberholverbot);

	//Rueckwege setzen
	WegHin->vSetRueckweg(WegRueck);
	WegRueck->vSetRueckweg(WegHin);

	//Kreuzungen setzen (Hinweg fuehrt zur zu verbindenden Kreuzung)
	WegRueck->vSetKreuzung(this);
	WegHin->vSetKreuzung(pKreuzung);

	//Setzt die Wege auf das Ende der Liste der jeweiligen Kreuzung
	p_pWege.push_back(WegHin);
	pKreuzung->vAddWeg(WegRueck);
}

//Tankt angenommene Autos, falls noch Benzin vorhanden
void Kreuzung::vTanken(Fahrzeug* pFzg)
{
	if (p_dTankstelle > 0)
	{
		p_dTankstelle -= pFzg->dTanken();
	}
}

//Stellt ein Auto aufgetankt und fahrend auf den ersten Weg auf der Liste
void Kreuzung::vAnnahme(Fahrzeug * pFzg, Weg* pWeg)
{
	vTanken(pFzg);

	Weg* pNeuerWeg = ptZufaelligerWeg(pWeg);
	pNeuerWeg->vAnnahme(pFzg);

	cout << "Umsetzen von " << pFzg->getName() << ":" << endl
		<< setw(9) << setfill(' ') << "ZEIT" << ":" << dGlobaleZeit << endl
		<< setw(9) << setfill(' ') << "KREUZUNG" << ":" << p_sName << "   Tankinhalt: " << p_dTankstelle << "l" << endl
		<< setw(9) << setfill(' ') << "WECHSEL" << ":" << pWeg->getName() << " -> " << pNeuerWeg->getName() << endl
		<< setw(9) << setfill(' ') << "FAHRZEUG" << ":" << pFzg->getName() << endl << endl;
}

//Stellt ein Auto aufgetankt und parkend auf den ersten Weg auf der Liste
void Kreuzung::vAnnahme(Fahrzeug * pFzg, double dStartzeit)
{
	vTanken(pFzg);

	(*p_pWege.begin())->vAnnahme(pFzg, dStartzeit);


}

//Fertigt alle von der Kreuzung abgehende Wege ab
void Kreuzung::vAbfertigung()
{
	list<Weg*>::iterator itL;
	
	for (itL = p_pWege.begin(); itL != p_pWege.end(); itL++)
	{
		(*itL)->vAbfertigung();
	}
}

//Setzt einen wegfuehrenden Weg ans Ende der Liste
void Kreuzung::vAddWeg(Weg* pWeg) { p_pWege.push_back(pWeg); }

//Ausgabe
ostream& Kreuzung::ostreamAusgabe(ostream& daten)
{
	AktivesVO::ostreamAusgabe(daten) << endl;
	int i = 1;

	for (list<Weg*>::iterator itL = p_pWege.begin(); p_pWege.end() != itL; itL++)
	{
		daten << i << ". Strasse: ( " << *(*itL) << " ) " << endl;
		i++;
	}

	//daten << ")";

	return daten;
}


/*
W�hlt einen zuf�lligen Weg aus der Liste aus, der nicht der R�ckweg des �bergebenen Weges ist, au�er es ist eine Sackgasse.
Daf�r durchl�uft es eine do-while Schleife in dem ein zuf�lliger Weg ausgew�hlt wird, bis einer gefunden wird, der nicht der R�ckweg ist.
*/
Weg* Kreuzung::ptZufaelligerWeg(Weg* pWeg)
{
	if (p_pWege.size() == 1)
	{
		return *(p_pWege.begin());
	}
	
	list<Weg*>::iterator itL;
	Weg* rueck = pWeg->vGetRueckweg();
	
	do 
	{
		itL = p_pWege.begin();
		int zahl = rand() % p_pWege.size();
		
		for (int i = 0; i < zahl; i++)
		{
			itL++;
		}

	} 
	while ((*itL)->getName() == rueck->getName());
	
	return *itL;

}