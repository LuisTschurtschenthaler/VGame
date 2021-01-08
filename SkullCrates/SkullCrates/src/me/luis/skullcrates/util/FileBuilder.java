package me.luis.skullcrates.util;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.Set;

import org.bukkit.configuration.ConfigurationSection;
import org.bukkit.configuration.file.YamlConfiguration;

public class FileBuilder {
	
	private File f;
	private YamlConfiguration c;
	
	public FileBuilder(String file) {
		this.f = new File(file);
		this.c = YamlConfiguration.loadConfiguration(this.f);
	}
	
	
	public FileBuilder setValue(String valuePath, Object value) {
		c.set(valuePath, value);
		return this;
	}

	public Object getObject(String valuePath) {
		return c.get(valuePath);
	}
	
	public int getInt(String valuePath) {
		return c.getInt(valuePath);
	}
	
	public String getString(String valuePath) {
		return c.getString(valuePath);
	}
	
	public boolean getBoolean(String valuePath) {
		return c.getBoolean(valuePath);
	}
	
	public List<String> getStringList(String valuePath) {
		return c.getStringList(valuePath);
	}
	
	public Set<String> getKeys(boolean deep) {
		return c.getKeys(deep);
	}
	
	public ConfigurationSection getConfigurationSection(String section) {
		return c.getConfigurationSection(section);
	}
	
	public void delete() {
		if(f.exists())
			f.delete();
	}
	
	public FileBuilder save() {
		try {
			c.save(this.f);
		} catch (IOException e) { }
		return this;
	}
}
