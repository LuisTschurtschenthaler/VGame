package me.luis.skullcrates.commands;

import org.bukkit.Bukkit;
import org.bukkit.command.Command;
import org.bukkit.command.CommandExecutor;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;

import me.luis.skullcrates.enums.Messages;
import me.luis.skullcrates.util.Crate;
import me.luis.skullcrates.util.Util;

public class CrateCommands implements CommandExecutor {
	
	private Crate crate;

	@Override
	public boolean onCommand(CommandSender s, Command c, String label, String[] args) {
		if(!(s instanceof Player))
			return false;
		
		Player p = (Player) s;
		
		if(args.length <= 0) {
			for(String msg : Messages.HELP.getMessages())
				p.sendMessage(msg);
			return true;
		}
		
		int amount;
					
		switch(args[0]) {
			case "help":
				if(args.length == 1)
					for(String msg : Messages.HELP.getMessages())
						p.sendMessage(msg);
				break;
			
			case "list":
				if(args.length != 1) {
					p.sendMessage(Messages.HELP_COMMMAND.getMessage());
					break;
				}
				
				if(Crate.getCrates().size() == 0) {
					p.sendMessage(Messages.NO_CRATES.getMessage());
					break;
				}
				
				String msg = "";
				for(Crate crate : Crate.getCrates())
					msg += "\n    &7\"" + crate.getDisplayname() + "&7\"&8,";
				
				msg = msg.substring(0, msg.length() - 1);
				p.sendMessage(Util.color("&7Verfügbare Crates&8: " + msg));
				break;
				
			//			0       1
			// /crate delete <crate>
			case "delete":
				if(args.length != 2) {
					p.sendMessage(Messages.HELP_COMMMAND.getMessage());
					break;
				}
				crate = Crate.getCrateFromString(args[1]);
				isCrate(p);
				crate.delete();
				p.sendMessage(Messages.CRATE_DELETED.getMessage());
				break;

			//          0        1
			// /crate additems <crate> 
			case "additems":
				if(args.length != 2) {
					p.sendMessage(Messages.HELP_COMMMAND.getMessage());
					break;
				}
				crate = Crate.getCrateFromString(args[1]);
				isCrate(p);
				crate.addItems(p.getInventory());
				p.sendMessage(Util.color("&7Items aus dem Inventar wurden zu \"" + crate.getDisplayname() + "&7\" hinzugefügt"));
				break;

				
			//          0      1         2			 3
			// /crate create <name> <displayname> <skinURL>
			case "create":
				if(args.length != 4) {
					p.sendMessage(Messages.HELP_COMMMAND.getMessage());
					return true;
				}
				
				crate = new Crate(args[1]);
				isCrate(p);
				crate.setItems(p.getInventory());
				crate.createLivedrop();
				crate.setDisplayname(args[2]);
				crate.create(args[3]);
				p.sendMessage(Util.color("&7Die Crate \"" + crate.getDisplayname() + "&7\" wurde erstellt"));
				break;
			
				
			//          0       1        2
			// /crate giveall <crate> <amount>
			case "giveall":
				if(args.length != 3)
					return true;
				
				amount = Integer.parseInt(args[2]);
				crate = Crate.getCrateFromString(args[1]);
				isCrate(p);
				for(Player pl : Bukkit.getServer().getOnlinePlayers())
					pl.getInventory().addItem(crate.getSkull(amount));
				p.sendMessage(Util.color("&7Gebe jedem Spieler \"" + crate.getDisplayname() + "&7\" " + Integer.toString(amount) + "x"));
				break;
					
			//          0    1         2       3
			// /crate give <crate> <player> <amount>
			case "give":
				if(args.length != 4) {
					p.sendMessage(Messages.HELP_COMMMAND.getMessage());
					return true;
				}
				
				if(!Util.isPlayerOnline(args[2])) {
					p.sendMessage(Messages.PLAYER_NOT_ONLINE.getMessage());
					return true;
				}
				
				Player pl = Bukkit.getServer().getPlayer(args[2]);
				amount = Integer.parseInt(args[3]);
				
				if(args[1].equals("*")) {
					for(Crate cr : Crate.getCrates())
						pl.getInventory().addItem(cr.getSkull(amount));

					p.sendMessage(Util.color("&7Gebe &b1x jede &7Crate"));
				} else {
					crate = Crate.getCrateFromString(args[1]);
					isCrate(p);
					pl.getInventory().addItem(crate.getSkull(amount));
					p.sendMessage(Util.color("&7Gebe \"" + crate.getDisplayname() + "&7\" " + Integer.toString(amount) + "x"));
				}
				break;
				
			default:
				p.sendMessage(Messages.HELP_COMMMAND.getMessage());
				break;
		}
		return false;
	}
	
	public void isCrate(Player p) {
		if(crate == null) {
			p.sendMessage(Messages.CRATE_NOT_FOUND.getMessage());
			return;
		}
	}
	
}
