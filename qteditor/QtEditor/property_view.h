#pragma once


#include <QDockWidget>
#include "core/array.h"
#include "core/string.h"
#include "core/resource.h"
#include "universe/entity.h"

namespace Lumix
{
	struct Component;
	struct Entity;
	class Event;
	class Path;
	class Resource;
	class WorldEditor;
}

namespace Ui
{
	class PropertyView;
}

class AssetBrowser;
class QTreeWidgetItem;
class ScriptCompiler;


class PropertyView : public QDockWidget
{
	Q_OBJECT

public:
	class Object
	{
		public:
			typedef Object* (*Creator)(Lumix::Resource*);

		public:
			Object(const char* name)
				: m_name(name)
			{ }
			virtual ~Object();
			const char* getName() const { return m_name.c_str(); }
			Object** getMembers() { return m_members.empty() ? NULL : &m_members[0]; }
			int getMemberCount() const { return m_members.size(); }
			void addMember(Object* member) { m_members.push(member); }

			virtual void createEditor(QTreeWidgetItem* item) = 0;

		private:
			Lumix::string m_name;
			Lumix::Array<Object*> m_members;
	};
	

	class Property
	{
		public:
			enum Type
			{
				FILE,
				STRING,
				DECIMAL,
				VEC3,
				BOOL,
				RESOURCE
			};
				
			Type m_type;
			uint32_t m_component;
			Lumix::string m_name;
			Lumix::string m_file_type;
			Lumix::string m_component_name;
			uint32_t m_name_hash;
			QTreeWidgetItem* m_tree_item;
	};

public:
	explicit PropertyView(QWidget* parent = NULL);
	~PropertyView();
	void setWorldEditor(Lumix::WorldEditor& editor);
	void setScriptCompiler(ScriptCompiler* compiler);
	void setAssetBrowser(AssetBrowser& asset_browser);
	void addResourcePlugin(Object::Creator plugin);
	Lumix::Resource* getSelectedResource() const { return m_selected_resource; }
	void setSelectedResource(Lumix::Resource* resource);
	void setObject(Object* object);

private slots:
	void on_addComponentButton_clicked();
	void on_checkboxStateChanged();
	void on_doubleSpinBoxValueChanged();
	void on_vec3ValueChanged();
	void on_lineEditEditingFinished();
	void on_browseFilesClicked();
	void on_goResourceClicked();
	void on_compileScriptClicked();
	void on_editScriptClicked();
	void on_animablePlayPause();
	void on_animableTimeSet(int value);
	void on_terrainBrushSizeChanged(int value);
	void on_terrainBrushStrengthChanged(int value);
	void on_TerrainHeightTypeClicked();
	void on_TerrainTextureTypeClicked();
	void on_terrainBrushTextureChanged(int value);
	void on_TerrainHeightSaveClicked();
	void on_TerrainSplatSaveClicked();
	void on_positionX_valueChanged(double arg1);
	void on_positionY_valueChanged(double arg1);
	void on_positionZ_valueChanged(double arg1);
	void on_propertyList_customContextMenuRequested(const QPoint &pos);

private:
	void createObjectEditor(QTreeWidgetItem* item, Object* object);
	void clear();
	void onUniverseCreated();
	void onUniverseDestroyed();
	void onEntitySelected(Lumix::Entity& e);
	void onEntityPosition(Lumix::Entity& e);
	void setSelectedResourceFilename(const char* filename);
	void addProperty(const char* component, const char* name, const char* label, Property::Type type, const char* file_type);
	void onPropertyValue(Property* property, const void* data, int32_t data_size);
	void addScriptCustomProperties();
	void addAnimableCustomProperties(const Lumix::Component& cmp);
	void addTerrainCustomProperties(const Lumix::Component& terrain_component);
	void onScriptCompiled(const Lumix::Path& path, uint32_t status);
	void setScriptStatus(uint32_t status);
	void updateValues();
	void updateSelectedEntityPosition();
	void onSelectedResourceLoaded(Lumix::Resource::State old_state, Lumix::Resource::State new_state);

private:
	Ui::PropertyView* m_ui;
	Lumix::Array<Property*> m_properties;
	ScriptCompiler* m_compiler;
	Lumix::Entity m_selected_entity;
	Lumix::WorldEditor* m_world_editor;
	bool m_is_updating_values;
	class TerrainEditor* m_terrain_editor;
	AssetBrowser* m_asset_browser;
	Lumix::Resource* m_selected_resource;
	Lumix::Array<Object::Creator> m_resource_plugins;
	Object* m_object;
};


