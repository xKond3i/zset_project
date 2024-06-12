console.log("%cCode written by " + "%cKonrad Ceglarski", "color: green", "color: green; font-weight: 900");

angular
.module("app", [])
.controller("controller",
function($scope) {
    $scope.data = {
        "status": true,
        "modes": [
            "rainbow", "wave", "solid", "breathing", "sound", "filling", "snake"
        ],
        "mode": 0,
        "options": [
            true, true, true
        ],
        "hue": 0,
        "sat": 0,
        "bright": 0,
    }
    $scope.load = function() {
        // Get Arduino data on load
        var xhr = new XMLHttpRequest();

        xhr.onreadystatechange = function() {
            // if our request is done and it's status is OK
            if (this.readyState == 4 && this.status == 200) {
                var data = JSON.parse(this.responseText);
                $scope.data = data;
            }
        }

        xhr.open("GET", "/data", true);
        xhr.send();
    }
    $scope.sendData = function() {
        // Send the data to Arduino
        var xhr = new XMLHttpRequest();

        xhr.open("POST", "/data");
        xhr.send(JSON.stringify($scope.data));
    }
    $scope.changeMode = function() {
        // Change LEDs mode
        $scope.data.mode = document.querySelector('input[name="mode-selection"]:checked').value;
        $scope.sendData();
    }
    $scope.changeOutput = function() {
        // Change Output field
        $scope.outputindicator = {"background-color": `hsl(${$scope.data.hue}, ${$scope.data.sat}%, ${$scope.data.bright}%)`};
        $scope.sendData();
    }
    $scope.statusSwitch = function() {
        // Turn on / off
        $scope.data.status = !$scope.data.status;
        if ($scope.data.status) {$scope.statuslight = {"background-color": "var(--enable)"};}
        else {$scope.statuslight = {"background-color": "var(--disable)"};}
        $scope.sendData();
    }
    $scope.hueSlider = function() {
        $scope.data.hue = $scope.hue;
        $scope.hueindicator = {"background-color": `hsl(${$scope.data.hue}, 100%, 50%)`};
        $scope.satstyle = {"background": `linear-gradient(to right, hsl(${$scope.data.hue}, 0%, 50%), hsl(${$scope.data.hue}, 100%, 50%))`}
        $scope.brightstyle = {"background": `linear-gradient(to right, hsl(${$scope.data.hue}, 100%, 0%), hsl(${$scope.data.hue}, 100%, 100%))`}
        $scope.changeOutput();
    }
    $scope.satSlider = function() {
        $scope.data.sat = $scope.sat;
        $scope.changeOutput();
    }
    $scope.brightSlider = function() {
        $scope.data.bright = $scope.bright;
        $scope.changeOutput();
    }
});