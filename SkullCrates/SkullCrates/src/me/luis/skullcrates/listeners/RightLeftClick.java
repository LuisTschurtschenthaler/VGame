package me.luis.skullcrates.listeners;

import org.bukkit.Material;
import org.bukkit.entity.Player;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.block.Action;
import org.bukkit.event.player.PlayerInteractEvent;
import org.bukkit.inventory.ItemStack;

import me.luis.skullcrates.enums.Messages;
import me.luis.skullcrates.util.Crate;
import me.luis.skullcrates.util.Util;

public class RightLeftClick implements Listener {
	
	private Crate crate;
	
	@EventHandler
	public void onPlayerUse(PlayerInteractEvent e) {
		Player p = e.getPlayer();
		ItemStack item = p.getInventory().getItemInMainHand();
		
		if(item.getType() != Material.SKULL_ITEM || item == null)
			return;
		
		if(!item.getItemMeta().getDisplayName().contains("Crate"))
			return;
					
		for(Crate c : Crate.getCrates()) {
			if(c.getDisplayname().equals(item.getItemMeta().getDisplayName())) {
				crate = c;
				break;
			}
		}
		
		if(e.getAction().equals(Action.RIGHT_CLICK_AIR) || e.getAction().equals(Action.RIGHT_CLICK_BLOCK)) {
			e.setCancelled(true);
			if(Util.getRunning()) {
				p.sendMessage(Messages.ALREADY_OPENING.getMessage());
				return;
			}
			
			p.openInventory(crate.openAccept());
		}
		
		if(e.getAction().equals(Action.LEFT_CLICK_AIR) || e.getAction().equals(Action.LEFT_CLICK_BLOCK)) {
			e.setCancelled(true);
			Util.setCurrentPage(1);
			p.openInventory(crate.openPreview(0));
		}
	}
}
