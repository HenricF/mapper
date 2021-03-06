/*
 *    Copyright 2012, 2013 Thomas Schöps
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


#ifndef _OPENORIENTEERING_TEMPLATE_MAP_H_
#define _OPENORIENTEERING_TEMPLATE_MAP_H_

#include "template.h"

#include <memory>

#include <QStringList>

/**
 * Template displaying a map file.
 */
class TemplateMap : public Template
{
Q_OBJECT
public:
	/**
	 * Returns the filename extensions supported by this template class.
	 */
	static const std::vector<QByteArray>& supportedExtensions();
	
	
	TemplateMap(const QString& path, Map* map);
	
	virtual ~TemplateMap() override;
	
	
	const char* getTemplateType() const override;
	
	bool isRasterGraphics() const override;
	
	
	bool loadTemplateFileImpl(bool configuring) override;
	
	bool postLoadConfiguration(QWidget* dialog_parent, bool& out_center_in_view) override;
	
	void unloadTemplateFileImpl() override;
	
	
	void drawTemplate(QPainter* painter, QRectF& clip_rect, double scale, bool on_screen, float opacity) const override;
	
	QRectF getTemplateExtent() const override;
	
	
	const Map* templateMap() const;
	
protected:
	Template* duplicateImpl() const override;
	
	Map* templateMap();
	
	void setTemplateMap(std::unique_ptr<Map>&& map);
	
private:
	std::unique_ptr<Map> template_map;
	
	static QStringList locked_maps;
};

#endif
