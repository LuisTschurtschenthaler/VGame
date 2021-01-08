package me.luis.skullcrates.enums;

import java.util.Arrays;
import java.util.List;

import me.luis.skullcrates.util.Util;

public enum Messages {
	
	ALREADY_OPENING("Already-Opening", "&7You're already opening a crate"),
	IS_FIRST_PAGE("Is-First-Page", "&7This is already the first page"),
	IS_LAST_PAGE("Is-Last-Page", "&7This is already the last page"),
	PLAYER_NOT_ONLINE("Player-Not-Online", "&7This player is &cnot &7online"),
	CRATE_NOT_FOUND("Crate-Not-Found", "&7This crate could &cnot &7be found"),
	CRATE_DELETED("Crate-Created", "&7The crate was successfully &cdeleted"),
	TOO_FEW_ITEMS("Too-Few-Items", "&7There are &ctoo few items &7in the crate"),
	NO_CRATES("No-Crates", "&7No crates have been created yet"),
	HELP_COMMMAND("Help-Command", "&7Use&8: &b/crate help"),
	HELP("Help", Arrays.asList("",
			"&8» &cCrates help&8:",
			"&8» &b/crate list &7- List all crates",
			"&8» &b/crate delete <crate> &7- Delete a crate",
			"&8» &b/crate additems <crate> &7- Add all items from the inventory to the crate",
			"&8» &b/crate create <name> <displayname> <skinURL> &7- Create a crate",
			"&8» &b/crate giveall <crate> <amount> &7- Give everyone a crate",
			"&8» &b/crate give <crate | *> <player> <amount> &7- Give one player a crates",
			"")
		);
	
	private String name;
	private String message;
	private List<String> messages;
	
	
	Messages(String name, String message) {
		this.name = name;
		this.message = message;
	}
	
	Messages(String name, List<String> messages) {
		this.name = name;
		this.messages = messages;
	}
	
	public String getName() {
		return Util.color(name);
	}
	
	public String getMessage() {
		return Util.color(message);
	}
	
	public List<String> getMessages() {
		return Util.colorLore(messages, null);
	}
	
	
}
