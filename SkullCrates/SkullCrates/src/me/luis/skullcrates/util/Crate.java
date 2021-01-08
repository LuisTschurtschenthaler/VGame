package me.luis.skullcrates.util;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Date;
import java.util.List;

import org.bukkit.Bukkit;
import org.bukkit.Material;
import org.bukkit.Sound;
import org.bukkit.entity.Player;
import org.bukkit.inventory.Inventory;
import org.bukkit.inventory.ItemStack;
import org.bukkit.inventory.meta.ItemMeta;
import org.bukkit.scheduler.BukkitRunnable;

import me.luis.skullcrates.SkullCrates;
import me.luis.skullcrates.enums.Messages;

public class Crate {

	public static SkullCrates skullcrates;

	private final static String HEAD_OAK_WOOD_LEFT = "8652e2b936ca8026bd28651d7c9f2819d2e923697734d18dfdb13550f8fdad5f";
	private final static String HEAD_OAK_WOOD_HASHTAG = "9ae85f74f8e2c054b781a29fa9b25934ba63bb79f1de8a95b436d9bfdcaf4cd";
	private final static String HEAD_OAK_WOOD_RIGHT = "2a3b8f681daad8bf436cae8da3fe8131f62a162ab81af639c3e0644aa6abac2f";

	private FileBuilder f;
	private String name;

	
	public Crate(String name) {
		this.f = new FileBuilder("plugins//SkullCrates//" + name + ".yml");
		this.name = name;
	}

	/* STATIC METHODS */
	public static List<Crate> getCrates() {
		List<Crate> temp = new ArrayList<>();
		File files = new File("plugins//SkullCrates//");
		
		for(File f : files.listFiles()) {
			if(f.isFile())
				temp.add(new Crate(f.getName().replace(".yml", "")));
		}
		
		return temp;
	}
	
	public static Crate getCrateFromString(String crateName) {
		for(Crate c : getCrates()) {
			if(c.getName().equalsIgnoreCase(crateName))
				return c;
		}
		return null;
	}
	

	/* CLASS METHODS */
	public String getName() {
		return this.name;
	}
	
	public void delete() {
		f.delete();
	}
	
	public Crate setDisplaySkull(String skull) {
		f.setValue("skull", skull.toString());
		return this;
	}
	
	public ItemStack getSkull(int amount) {
		return Util.getSkull(f.getString("skull"), amount, getDisplayname(), f.getStringList("description"), this);
	}
	
	public Crate setItems(Inventory inv) {
		f.setValue("items", inv.getContents());
		f.save();
		return this;
	}
	
	public Crate addItems(Inventory inv) {
		@SuppressWarnings("unchecked")
		List<ItemStack> temp = (List<ItemStack>) f.getObject("items");
		for(ItemStack i : inv.getContents()) {
			if(i != null)
				temp.add(i);
		}
		ItemStack[] content = temp.toArray(new ItemStack[0]);
		f.setValue("items", content);
		f.save();
		return this;
	}
	
	public Crate setDisplayname(String displayname) {
		f.setValue("displayname", "&8➤ &7&lCrate &8❘ " + displayname);
		f.save();
		return this;
	}
	
	public String getDisplayname() {
		return Util.color(f.getString("displayname"));
	}

	public Crate createLivedrop() {
		List<ItemStack> temp = new ArrayList<>();
		for(int i = 0; i != 5; i++)
			temp.add(new ItemBuilder(Material.BARRIER, 1).setDisplayname("&cNo item found").setLore(Arrays.asList("&7No item has been drawn since the last restart")).build());
		
		f.setValue("livedrop.items", temp);
		f.save();
		return this;
	}
	
	public Crate deleteLiveDrop() {
		f.setValue("livedrop.items", null);
		f.save();
		return this;
	}
	
	public List<ItemStack> getLivedrop() {
		List<ItemStack> temp = new ArrayList<>();
		@SuppressWarnings("unchecked")
		ItemStack[] content = ((List<ItemStack>) f.getObject("livedrop.items")).toArray(new ItemStack[0]);
		
		for(int i = 0; i < content.length; i++) {
			if(content[i] != null)
				temp.add(content[i]);
		}
		return temp;
	}
	
	public Crate updateLivedrop(Player p, ItemStack i) {
		List<ItemStack> temp = getLivedrop();
		String t = i.getItemMeta().getDisplayName() == null ? i.getType().toString() : i.getItemMeta().getDisplayName();
		SimpleDateFormat days = new SimpleDateFormat("dd/MM/yyyy");
		SimpleDateFormat time = new SimpleDateFormat("HH:mm:ss");
	    Date date = new Date(); 
		
		temp.add(0, new ItemBuilder(i).setDisplayname("&3&l" + t).setLore(Arrays.asList("",
						"&7Player&8: &b" + p.getDisplayName(),
						"&7Drawn on &b" + days.format(date) + " &7at &b" + time.format(date))).build());
		
		while(temp.size() > 5)
			temp.remove(temp.size() - 1);
		
		while(temp.size() < 5)
			temp.add(new ItemBuilder(Material.BARRIER, 1).setDisplayname("&cNo item found").setLore(Arrays.asList("&7No item has been drawn since the last restart")).build());

		f.setValue("livedrop.items", null);
		f.setValue("livedrop.items", temp);
		f.save();
		return this;
	}
	
	public Inventory openLivedrop() {
		List<ItemStack> temp = getLivedrop();
		List<Integer> itemSlots = Arrays.asList(11, 12, 13, 14, 15);
		
		Inventory inv = Bukkit.createInventory(null, 9*3, "Livedrop: " + getDisplayname());
		
		int item = 0;
		for(int i = 0; i < inv.getSize(); i++) {
			if(itemSlots.contains(i)) {
				inv.setItem(i, temp.get(item));
				item++;
			}
			else inv.setItem(i, new ItemBuilder(Material.STAINED_GLASS_PANE, 1, (short) 15).setDisplayname("§7").build());
		}
		
		return inv;
	}
	
	public List<ItemStack> getItems() {
		List<ItemStack> temp = new ArrayList<>();
		@SuppressWarnings("unchecked")
		ItemStack[] content = ((List<ItemStack>) f.getObject("items")).toArray(new ItemStack[0]);
	
		int amountofItems = 0;
		for(int i = 0; i < content.length; i++) {
			if(content[i] != null)
				amountofItems++;
		}
		
		if(amountofItems <= 0) return temp;
		
		double chance = 100 / amountofItems;
		for(int i = 0; i < content.length; i++) {
			if(content[i] != null) {
				List<String> itemLore = content[i].getItemMeta().getLore();
				
				if(itemLore != null && !itemLore.get(itemLore.size() - 1).contains("Chance")) {
					itemLore.add("&7Chance&8: &b~" + Double.toString(chance) + "%");
					Util.addItemLore(content[i], itemLore);
				
				} else Util.setLore(content[i], Arrays.asList("&7Chance&8: &b~" + Double.toString(chance) + "%"));

				temp.add(content[i]);
			}
		}
	
		return temp;
	}
	
	public Crate create(String skullURL) {
		f.setValue("skull", skullURL);
		f.setValue("description", Arrays.asList("",
				"&8» &7In crate are &3%amount% &7items",
				"&8» &All information about the Crate with &3left-click",
				"",
				"&8» &bRight-click &8❘ &7Opens the crate",
				"&8» &bLeft-click &8❘ &7Opens the preview",
				""));
		f.save();
		return this;
	}
	
	public Inventory openAccept() {
		Inventory temp = Bukkit.createInventory(null, 9*3, "Crateconfirmation:");

		for(int i = 0; i < temp.getSize(); i++)
			temp.setItem(i, new ItemBuilder(Material.STAINED_GLASS_PANE, 1, (short) 15).setDisplayname("§7").build());
		
		// Accept
		temp.setItem(9, new ItemBuilder(Material.STAINED_CLAY, 1, (short) 5).setDisplayname("&aOpen").build());
		temp.setItem(10, new ItemBuilder(Material.STAINED_CLAY, 1, (short) 5).setDisplayname("&aOpen").build());
		temp.setItem(11, new ItemBuilder(Material.STAINED_CLAY, 1, (short) 5).setDisplayname("&aOpen").build());
		
		// Info
		temp.setItem(13, new ItemBuilder(Material.EMPTY_MAP, 1).setDisplayname("&7Do you want to open the crate?").build());
		
		// Decline
		temp.setItem(15, new ItemBuilder(Material.STAINED_CLAY, 1, (short) 14).setDisplayname("&cClose").build());
		temp.setItem(16, new ItemBuilder(Material.STAINED_CLAY, 1, (short) 14).setDisplayname("&cClose").build());
		temp.setItem(17, new ItemBuilder(Material.STAINED_CLAY, 1, (short) 14).setDisplayname("&cClose").build());
		
		return temp;
	}
	
	public void startAnimation(Player p, boolean isTest) {
		ItemStack item = p.getInventory().getItemInMainHand();	
		List<ItemStack> temp = getItems();
		Collections.shuffle(temp);
		
		if(temp.size() < 7) {
			p.sendMessage(Messages.TOO_FEW_ITEMS.getMessage());
			return;
		}
		
		if(!isTest) {
			if(item.getAmount() > 1)
				item.setAmount(item.getAmount() - 1);
			
			else p.getInventory().setItemInMainHand(null);
		}
		
		Util.setRunning(true);
		Inventory inv = Bukkit.createInventory(null, 9 * 3, getDisplayname());
		
		for(int i = 0; i < inv.getSize(); i++)
			inv.setItem(i, new ItemBuilder(Material.STAINED_GLASS_PANE, 1, (short) 15).setDisplayname("§7").build());
		
		inv.setItem(4, new ItemBuilder(Material.HOPPER, 1).setDisplayname("&b&lYour win")
				.setLore(Arrays.asList("&8» &7Win this item")).build());
		
		p.openInventory(inv);
		new BukkitRunnable() {
			
			private final Player curr = p;
			private long sleep = 0;
			
			@Override
			public void run() {
				if(curr == null) this.cancel();

				p.openInventory(inv);
				
				int id = 0;
				for(int i = 10; i <= 16; i++) {
					if(temp.get(id) != null)
						inv.setItem(i, temp.get(id));
					id++;
				}
				
				p.playSound(p.getLocation(), Sound.UI_BUTTON_CLICK, 10F, 10F);
				
				temp.add(temp.get(0));
				temp.remove(0);
				
				if(p.getOpenInventory() == null)
					p.openInventory(inv);
				
				sleep += 50;
				
				if(sleep == 1000) {
					this.cancel();
					ItemStack win = inv.getItem(12);
					
					List<String> lore = win.getItemMeta().getLore();
					lore.remove(lore.size() - 1);

					ItemMeta meta = win.getItemMeta();
					meta.setLore(Util.colorLore(lore, null));
					win.setItemMeta(meta);
					
					if(!isTest) {
						String t = win.getItemMeta().getDisplayName() == null ? win.getType().toString() : win.getItemMeta().getDisplayName();
						p.getInventory().addItem(win);
						p.sendMessage(Util.color("&7You pulled &3&l" + t + " &7from " + getDisplayname()));
						updateLivedrop(p, win);
					
					} else p.sendMessage(Util.color("&7If this were a real crate, you would have won&8:\n &b" + win.getAmount() + "x " + win.getItemMeta().getDisplayName()));
					
					p.closeInventory();
					Util.setRunning(false);
				}
			};
		}.runTaskTimer(skullcrates, 0, 3);
	}	
	
	public Inventory openPreview(int startPos) {
		List<Integer> border = Arrays.asList(0, 8, 9, 17, 18, 26, 27, 35, 36);
		List<String> lore = Arrays.asList();
		List<ItemStack> temp = getItems();
		
		Inventory inv = Bukkit.createInventory(null, 9*6, "Content: " + getDisplayname());
		
		int item = startPos;
		int slot = 0;
		
		while(item != temp.size() && slot < 44) {
			if(!border.contains(slot)) {
				inv.setItem(slot, temp.get(item));
				item++;
			}
			slot++;
		}

		inv.setItem(0, Util.getSkull(f.getString("skull"), 1, Util.color("&aFree test-spin"), 
				Arrays.asList("&7Try this crate for free", "",
				"&cThis is a preview.",
				"&cYou do not receive the item."), this));
		inv.setItem(9, Util.getSkull(f.getString("skull"), 1, Util.color("&f&lCrate Livedrop"), Arrays.asList("&7Shows you the live drop of the crate"), this));
		
		inv.setItem(48, Util.getSkull(HEAD_OAK_WOOD_LEFT, 1, Util.color("&8» &c&oBack"), lore, this));
		inv.setItem(49, Util.getSkull(HEAD_OAK_WOOD_HASHTAG, 1, 
				Util.color("&8» &7Page&8: &a") + Integer.toString(Util.getCurrentPage()), lore, this));
		inv.setItem(50, Util.getSkull(HEAD_OAK_WOOD_RIGHT, 1, Util.color("&8» &c&oNext page"), lore, this));
		return inv;
	}
}
