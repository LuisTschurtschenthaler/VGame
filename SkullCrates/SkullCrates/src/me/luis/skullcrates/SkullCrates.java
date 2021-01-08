package me.luis.skullcrates;

import org.bukkit.plugin.PluginManager;
import org.bukkit.plugin.java.JavaPlugin;

import me.luis.skullcrates.commands.CrateCommands;
import me.luis.skullcrates.listeners.ClickEvent;
import me.luis.skullcrates.listeners.RightLeftClick;
import me.luis.skullcrates.util.Crate;

public class SkullCrates extends JavaPlugin {
	
	@Override
	public void onEnable() {
		// Register events
		PluginManager pm = getServer().getPluginManager();
		pm.registerEvents(new ClickEvent(), this);
		pm.registerEvents(new RightLeftClick(), this);
		
		// Register commands
		getCommand("crate").setExecutor(new CrateCommands());
		
		//Crate.loadCrates();
		Crate.skullcrates = this;
	}
	
	
	@Override
	public void onDisable() {
		
	}
}
