/*
 *    Copyright 2012, 2013 Pete Curtis
 *
 *    This file is part of OpenOrienteering.
 *
 *    OpenOrienteering is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    OpenOrienteering is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with OpenOrienteering.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "file_import_export.h"

#include <QFileInfo>

#include "map.h"
#include "symbol.h"
#include "template.h"
#include "object.h"
#include "symbol_line.h"
#include "symbol_point.h"


// ### ImportExport ###

ImportExport::~ImportExport()
{
	// Nothing, not inlined
}


// ### Importer ###

Importer::~Importer()
{
	// Nothing, not inlined
}

void Importer::doImport(bool load_symbols_only, const QString& map_path) throw (FileFormatException)
{
	import(load_symbols_only);
	
	// Object post processing:
	// - make sure that there is no object without symbol
	// - make sure that all area-only path objects are closed
	// - make sure that there are no special points in wrong places (e.g. curve starts inside curves)
	for (int p = 0; p < map->getNumParts(); ++p)
	{
		MapPart* part = map->getPart(p);
		for (int o = 0; o < part->getNumObjects(); ++o)
		{
			Object* object = part->getObject(o);
			if (object->getSymbol() == NULL)
			{
				addWarning(Importer::tr("Found an object without symbol."));
				if (object->getType() == Object::Point)
					object->setSymbol(map->getUndefinedPoint(), true);
				else if (object->getType() == Object::Path)
					object->setSymbol(map->getUndefinedLine(), true);
				else
				{
					// There is no undefined symbol for this type of object, delete the object
					part->deleteObject(o, false);
					--o;
					continue;
				}
			}
			
			if (object->getType() == Object::Path)
			{
				PathObject* path = object->asPath();
				Symbol::Type contained_types = path->getSymbol()->getContainedTypes();
				if (contained_types & Symbol::Area && !(contained_types & Symbol::Line))
					path->closeAllParts();
				
				for (int i = 0; i < path->getCoordinateCount(); ++i)
				{
					if (!path->getCoordinate(i).isCurveStart())
						continue;
					
					if (i >= path->getCoordinateCount() - 3)
					{
						path->getCoordinate(i).setCurveStart(false);
						continue;
					}
					if (path->getCoordinate(i + 1).isClosePoint() || path->getCoordinate(i + 1).isHolePoint() ||
						path->getCoordinate(i + 2).isClosePoint() || path->getCoordinate(i + 2).isHolePoint())
					{
						path->getCoordinate(i).setCurveStart(false);
						continue;
					}
					
					path->getCoordinate(i + 1).setCurveStart(false);
					path->getCoordinate(i + 1).setDashPoint(false);
					path->getCoordinate(i + 2).setCurveStart(false);
					path->getCoordinate(i + 2).setDashPoint(false);
					i += 2;
				}
			}
		}
	}
	
	// Symbol post processing
	for (int i = 0; i < map->getNumSymbols(); ++i)
	{
		if (!map->getSymbol(i)->loadFinished(map))
			throw FileFormatException(Importer::tr("Error during symbol post-processing."));
	}
	
	// Template loading: try to find all template files
	bool have_lost_template = false;
	for (int i = 0; i < map->getNumTemplates(); ++i)
	{
		Template* temp = map->getTemplate(i);
		
		bool loaded_from_template_dir = false;
		temp->tryToFindAndReloadTemplateFile(map_path, &loaded_from_template_dir);
		if (loaded_from_template_dir)
			addWarning(Importer::tr("Template \"%1\" has been loaded from the map's directory instead of the relative location to the map file where it was previously.").arg(temp->getTemplateFilename()));
		
		if (temp->getTemplateState() != Template::Loaded)
			have_lost_template = true;
	}
	if (have_lost_template)
		addWarning(Importer::tr("At least one template file could not be found. Click the red template name(s) in the Templates -> Template setup window to locate the template file name(s)."));
}

void Importer::finishImport() throw (FileFormatException)
{
	// Nothing, not inlined
}


// ### Exporter ###

Exporter::~Exporter()
{
	// Nothing, not inlined
}