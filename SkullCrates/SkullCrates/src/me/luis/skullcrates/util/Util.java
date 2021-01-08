package me.luis.skullcrates.util;

import java.lang.reflect.Field;
import java.util.List;
import java.util.UUID;

import org.apache.commons.codec.binary.Base64;
import org.bukkit.Bukkit;
import org.bukkit.ChatColor;
import org.bukkit.Material;
import org.bukkit.entity.Player;
import org.bukkit.inventory.ItemStack;
import org.bukkit.inventory.meta.ItemMeta;
import org.bukkit.inventory.meta.SkullMeta;

import com.mojang.authlib.GameProfile;
import com.mojang.authlib.properties.Property;

import net.md_5.bungee.api.ChatMessageType;
import net.md_5.bungee.api.chat.TextComponent;

public class Util {
	private static boolean isRunning = false;
	private static int currentPage = 1;

	public static void setRunning(boolean bool) {
		isRunning = bool;
	}
	
	public static boolean getRunning() {
		return isRunning;
	}
	
	public static void setCurrentPage(int page) {
		currentPage = page;
	}
	
	public static int getCurrentPage() {
		return currentPage;
	}
	
	public static String color(String string) {
		return ChatColor.translateAlternateColorCodes('&', string);
	}
	
	public static void sendMessage(Player p, List<String> messages) {
		for(String message : messages)
			p.sendMessage(message);
	}

	public static void sendBroadcast(List<String> messages) {
		for(String message : messages)
			Bukkit.broadcastMessage(message);
	}
	
	 
	public static List<String> colorLore(List<String> itemLore, Crate crate) {
		for (int line = 0; line < itemLore.size(); line++) {
			if(crate != null)
				itemLore.set(line, itemLore.get(line).replaceAll("%amount%", Integer.toString(crate.getItems().size())));
			itemLore.set(line, color(itemLore.get(line)));
		}
		return itemLore;
	}
	
	public static ItemStack addItemLore(ItemStack i, List<String> lore) {
		ItemMeta m = i.getItemMeta();
		
		m.setLore(colorLore(lore, null));
		i.setItemMeta(m);
		return i;
	}
	
	public static ItemStack getSkull(String skullURL, int amount, String name, List<String> lore, Crate c) {
		ItemStack head = new ItemStack(Material.SKULL_ITEM, amount, (short) 3);
        SkullMeta headMeta = (SkullMeta) head.getItemMeta();
        
        headMeta.setLore(colorLore(lore, c));
        headMeta.setDisplayName(Util.color(name));
        
        if(skullURL.isEmpty()) {
        	head.setItemMeta(headMeta);
        	return head;
        }
       
        GameProfile profile = new GameProfile(UUID.randomUUID(), null);
       
        byte[] encodedData = Base64.encodeBase64(String.format("{textures:{SKIN:{url:\"http://textures.minecraft.net/texture/%s\"}}}", skullURL).getBytes());
        profile.getProperties().put("textures", new Property("textures", new String(encodedData)));
        
        try {
        	Field profileField = headMeta.getClass().getDeclaredField("profile");
            profileField.setAccessible(true);
            profileField.set(headMeta, profile);
        
        } catch (Exception e) {
            e.printStackTrace();
        }
        head.setItemMeta(headMeta);
        return head;
	}
	
	public static boolean isPlayerOnline(String player) {
		for(Player p : Bukkit.getServer().getOnlinePlayers()) {
			if(p.getName().equalsIgnoreCase(player))
				return true;
		}
		return false;
	}
	
	public static Player getPlayer(String player) {
		for(Player p : Bukkit.getServer().getOnlinePlayers()) {
			if(p.getDisplayName().equalsIgnoreCase(player))
				return p;
		}
		return null;
	}
	
	public ItemStack addLore(ItemStack i, List<String> lore) {
		ItemMeta meta = i.getItemMeta();
		List<String> oldLore = meta.getLore();
		
		lore.addAll(lore);
		meta.setLore(colorLore(oldLore, null));
		
		i.setItemMeta(meta);
		return i;
	}
	
	public static ItemStack setLore(ItemStack i, List<String> lore) {
		ItemMeta meta = i.getItemMeta();
		meta.setLore(colorLore(lore, null));
		i.setItemMeta(meta);
		return i;
	}
}
