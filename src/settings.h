/*
 *    Copyright 2012 Thomas Schöps
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


#ifndef _OPENORIENTEERING_SETTINGS_H_
#define _OPENORIENTEERING_SETTINGS_H_

#include <QObject>
#include <QHash>
#include <QVariant>

/** Singleton which handles the global application settings.
 *  If you need to do any action when the application settings are changed, connect to the settingsChanged() signal.
 */
class Settings : public QObject
{
Q_OBJECT
public:
	/// List of all application settings to prevent having the QSettings path strings in multiple places
	enum SettingsEnum
	{
		MapDisplay_Antialiasing = 0,
		MapEditor_ClickTolerance,
		MapEditor_ChangeSymbolWhenSelecting,
		MapEditor_ZoomOutAwayFromCursor,
		General_Language
	};
	
	/// Retrieve a setting from QSettings without caching
	QVariant getSetting(SettingsEnum setting) const;
	
	/// Can be used to cache and retrieve settings which are only changed in the settings dialog (i.e. applySettings() must be called after they are changed)
	QVariant getSettingCached(SettingsEnum setting);
	
	/// This must be called after the settings have been changed or on application startup.
	void applySettings();
	
	// TODO: Methods for settings import / export?
	
	/// Returns the path to use with QSettings
	inline QString getSettingPath(SettingsEnum setting) const { return setting_paths[setting]; }
	/// Returns the default value for the setting
	inline QVariant getDefaultValue(SettingsEnum setting) const { return setting_defaults[setting]; }
	
	static Settings& getInstance()
	{
		static Settings instance;
		return instance;
	}
	
signals:
	void settingsChanged();
	
private:
	Settings();
	void registerSetting(SettingsEnum id, const QString& path, const QVariant& default_value);
	
	QHash<SettingsEnum, QVariant> settings_cache;
	QHash<SettingsEnum, QString> setting_paths;
	QHash<SettingsEnum, QVariant> setting_defaults;
};

#endif