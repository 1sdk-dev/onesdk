/**
 * Copyright 2023 xin.yang
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 *     http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

package tech.mxin.onesdk.framework;

import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;

public interface IActivityLifeCycle {

    default void onActivityResult(int requestCode, int resultCode, Intent data) {
    }

    default void onStart() {
    }

    default void onPause() {
    }

    default void onResume() {
    }

    default void onNewIntent(Intent intent) {
    }

    default void onStop() {
    }

    default void onDestroy() {
    }

    default void onRestart() {
    }

    default void onBackPressed() {
    }

    default void onConfigurationChanged(Configuration newConfig) {
    }

    default void onRestoreInstanceState(Bundle savedInstanceState) {
    }

    default void onSaveInstanceState(Bundle outState) {
    }

    default void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
    }

}
