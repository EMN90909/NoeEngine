/**************************************************************************/
/*  NoeEnginePluginRegistry.java                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             NOEENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

package org.noeengine.noeengine.plugin;

import org.noeengine.noeengine.NoeEngine;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.Nullable;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Registry used to load and access the registered NoeEngine Android plugins.
 */
public final class NoeEnginePluginRegistry {
	private static final String TAG = NoeEnginePluginRegistry.class.getSimpleName();

	private static final String NOEENGINE_PLUGIN_V1_NAME_PREFIX = "org.noeengine.plugin.v1.";

	private static NoeEnginePluginRegistry instance;
	private final ConcurrentHashMap<String, NoeEnginePlugin> registry;

	private NoeEnginePluginRegistry(NoeEngine noeengine) {
		registry = new ConcurrentHashMap<>();
		loadPlugins(noeengine);
	}

	/**
	 * Retrieve the plugin tied to the given plugin name.
	 * @param pluginName Name of the plugin
	 * @return {@link NoeEnginePlugin} handle if it exists, null otherwise.
	 */
	@Nullable
	public NoeEnginePlugin getPlugin(String pluginName) {
		return registry.get(pluginName);
	}

	/**
	 * Retrieve the full set of loaded plugins.
	 */
	public Collection<NoeEnginePlugin> getAllPlugins() {
		return registry.values();
	}

	/**
	 * Parse the manifest file and load all included NoeEngine Android plugins.
	 * <p>
	 * A plugin manifest entry is a '<meta-data>' tag setup as described in the {@link NoeEnginePlugin}
	 * documentation.
	 *
	 * @param noeengine NoeEngine instance
	 * @return A singleton instance of {@link NoeEnginePluginRegistry}. This ensures that only one instance
	 * of each NoeEngine Android plugins is available at runtime.
	 */
	public static NoeEnginePluginRegistry initializePluginRegistry(NoeEngine noeengine) {
		if (instance == null) {
			instance = new NoeEnginePluginRegistry(noeengine);
		}

		return instance;
	}

	/**
	 * Return the plugin registry if it's initialized.
	 * Throws a {@link IllegalStateException} exception if not.
	 *
	 * @throws IllegalStateException if {@link NoeEnginePluginRegistry#initializePluginRegistry(NoeEngine)} has not been called prior to calling this method.
	 */
	public static NoeEnginePluginRegistry getPluginRegistry() throws IllegalStateException {
		if (instance == null) {
			throw new IllegalStateException("Plugin registry hasn't been initialized.");
		}

		return instance;
	}

	private void loadPlugins(NoeEngine noeengine) {
		try {
			final Activity activity = noeengine.getActivity();
			ApplicationInfo appInfo = activity
											  .getPackageManager()
											  .getApplicationInfo(activity.getPackageName(),
													  PackageManager.GET_META_DATA);
			Bundle metaData = appInfo.metaData;
			if (metaData == null || metaData.isEmpty()) {
				return;
			}

			int noeenginePluginV1NamePrefixLength = NOEENGINE_PLUGIN_V1_NAME_PREFIX.length();
			for (String metaDataName : metaData.keySet()) {
				// Parse the meta-data looking for entry with the NoeEngine plugin name prefix.
				if (metaDataName.startsWith(NOEENGINE_PLUGIN_V1_NAME_PREFIX)) {
					String pluginName = metaDataName.substring(noeenginePluginV1NamePrefixLength).trim();
					Log.i(TAG, "Initializing NoeEngine plugin " + pluginName);

					// Retrieve the plugin class full name.
					String pluginHandleClassFullName = metaData.getString(metaDataName);
					if (!TextUtils.isEmpty(pluginHandleClassFullName)) {
						try {
							// Attempt to create the plugin init class via reflection.
							@SuppressWarnings("unchecked")
							Class<NoeEnginePlugin> pluginClass = (Class<NoeEnginePlugin>)Class
																	 .forName(pluginHandleClassFullName);
							Constructor<NoeEnginePlugin> pluginConstructor = pluginClass
																				 .getConstructor(NoeEngine.class);
							NoeEnginePlugin pluginHandle = pluginConstructor.newInstance(noeengine);

							// Load the plugin initializer into the registry using the plugin name as key.
							if (!pluginName.equals(pluginHandle.getPluginName())) {
								Log.w(TAG,
										"Meta-data plugin name does not match the value returned by the plugin handle: " + pluginName + " =/= " + pluginHandle.getPluginName());
							}
							registry.put(pluginName, pluginHandle);
							Log.i(TAG, "Completed initialization for NoeEngine plugin " + pluginHandle.getPluginName());
						} catch (ClassNotFoundException e) {
							Log.w(TAG, "Unable to load NoeEngine plugin " + pluginName, e);
						} catch (IllegalAccessException e) {
							Log.w(TAG, "Unable to load NoeEngine plugin " + pluginName, e);
						} catch (InstantiationException e) {
							Log.w(TAG, "Unable to load NoeEngine plugin " + pluginName, e);
						} catch (NoSuchMethodException e) {
							Log.w(TAG, "Unable to load NoeEngine plugin " + pluginName, e);
						} catch (InvocationTargetException e) {
							Log.w(TAG, "Unable to load NoeEngine plugin " + pluginName, e);
						}
					} else {
						Log.w(TAG, "Invalid plugin loader class for " + pluginName);
					}
				}
			}
		} catch (PackageManager.NameNotFoundException e) {
			Log.e(TAG, "Unable load NoeEngine Android plugins from the manifest file.", e);
		}
	}
}
