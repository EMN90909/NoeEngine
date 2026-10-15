/**************************************************************************/
/*  FullScreenNoeEngineApp.java                                               */
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

package org.noeengine.noeengine;

import org.noeengine.noeengine.utils.ProcessPhoenix;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.CallSuper;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;

/**
 * Base activity for Android apps intending to use NoeEngine as the primary and only screen.
 *
 * It's also a reference implementation for how to setup and use the {@link NoeEngine} fragment
 * within an Android app.
 */
public abstract class FullScreenNoeEngineApp extends FragmentActivity implements NoeEngineHost {
	private static final String TAG = FullScreenNoeEngineApp.class.getSimpleName();

	@Nullable
	private NoeEngine noeengineFragment;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.noeengine_app_layout);

		Fragment currentFragment = getSupportFragmentManager().findFragmentById(R.id.noeengine_fragment_container);
		if (currentFragment instanceof NoeEngine) {
			Log.v(TAG, "Reusing existing NoeEngine fragment instance.");
			noeengineFragment = (NoeEngine)currentFragment;
		} else {
			Log.v(TAG, "Creating new NoeEngine fragment instance.");
			noeengineFragment = initNoeEngineInstance();
			getSupportFragmentManager().beginTransaction().replace(R.id.noeengine_fragment_container, noeengineFragment).setPrimaryNavigationFragment(noeengineFragment).commitNowAllowingStateLoss();
		}
	}

	@Override
	public void onDestroy() {
		Log.v(TAG, "Destroying NoeEngine app...");
		super.onDestroy();
		terminateNoeEngineInstance(noeengineFragment);
	}

	@Override
	public final void onNoeEngineForceQuit(NoeEngine instance) {
		runOnUiThread(() -> {
			terminateNoeEngineInstance(instance);
		});
	}

	private void terminateNoeEngineInstance(NoeEngine instance) {
		if (instance == noeengineFragment) {
			Log.v(TAG, "Force quitting NoeEngine instance");
			ProcessPhoenix.forceQuit(FullScreenNoeEngineApp.this);
		}
	}

	@Override
	public final void onNoeEngineRestartRequested(NoeEngine instance) {
		runOnUiThread(() -> {
			if (instance == noeengineFragment) {
				// It's very hard to properly de-initialize NoeEngine on Android to restart the game
				// from scratch. Therefore, we need to kill the whole app process and relaunch it.
				//
				// Restarting only the activity, wouldn't be enough unless it did proper cleanup (including
				// releasing and reloading native libs or resetting their state somehow and clearing static data).
				Log.v(TAG, "Restarting NoeEngine instance...");
				ProcessPhoenix.triggerRebirth(FullScreenNoeEngineApp.this);
			}
		});
	}

	@Override
	public void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		if (noeengineFragment != null) {
			noeengineFragment.onNewIntent(intent);
		}
	}

	@CallSuper
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (noeengineFragment != null) {
			noeengineFragment.onActivityResult(requestCode, resultCode, data);
		}
	}

	@CallSuper
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		if (noeengineFragment != null) {
			noeengineFragment.onRequestPermissionsResult(requestCode, permissions, grantResults);
		}
	}

	@Override
	public void onBackPressed() {
		if (noeengineFragment != null) {
			noeengineFragment.onBackPressed();
		} else {
			super.onBackPressed();
		}
	}

	/**
	 * Used to initialize the NoeEngine fragment instance in {@link FullScreenNoeEngineApp#onCreate(Bundle)}.
	 */
	@NonNull
	protected NoeEngine initNoeEngineInstance() {
		return new NoeEngine();
	}

	@Nullable
	protected final NoeEngine getNoeEngineFragment() {
		return noeengineFragment;
	}
}
