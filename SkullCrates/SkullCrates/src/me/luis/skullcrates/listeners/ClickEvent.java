package me.luis.skullcrates.listeners;

import org.bukkit.entity.Player;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.inventory.InventoryClickEvent;
import org.bukkit.inventory.ItemStack;

import me.luis.skullcrates.enums.Messages;
import me.luis.skullcrates.util.Crate;
import me.luis.skullcrates.util.Util;

public class ClickEvent implements Listener {
	
	private Crate crate;

	@EventHandler
	public void onInventoryClick(InventoryClickEvent e) {
		if(!e.getInventory().getTitle().contains("Crate"))
			return;
		e.setCancelled(true);
		
		Player p = (Player) e.getWhoClicked();
		ItemStack i = p.getInventory().getItemInMainHand();
		
		for(Crate c : Crate.getCrates()) {
			if(i.getItemMeta().getDisplayName().equals(c.getDisplayname())) {
				crate = c;
				break;
			}
		}
		
		int slot = e.getSlot();
		if(e.getInventory().getTitle().contains("Content")) {
			switch(slot) {
				case 0:
					Util.setCurrentPage(1);
					crate.startAnimation(p, true);
					break;
				
				case 9:
					p.openInventory(crate.openLivedrop());
					break;
				
				case 48:
					if(Util.getCurrentPage() != 1) {
						Util.setCurrentPage(1);
						p.openInventory(crate.openPreview(0));
					
					} else p.sendMessage(Messages.IS_FIRST_PAGE.getMessage());
					break;
				
				case 50:
					if(crate.getItems().size() > 5*7 && Util.getCurrentPage() != 2) {
						Util.setCurrentPage(2);
						p.openInventory(crate.openPreview(5*7));
					
					} else p.sendMessage(Messages.IS_LAST_PAGE.getMessage());
					break;
			}
		} 
		
		if(e.getInventory().getTitle().contains("Crateconfirmation")) {
			if(slot >= 9 && slot <= 11)
				crate.startAnimation(p, false);
			else if(slot >= 15 && slot <= 17)
				p.closeInventory();
		}
	}
}
