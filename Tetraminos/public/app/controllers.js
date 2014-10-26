'use strict';

/* Controllers */

angular.module('myApp.controllers', [])
    .controller('AppCtrl', ['$scope', '$location', '$http', '$routeParams',
        function ($scope, $location, $http, $routeParams) {

            $scope.message = undefined;


            $scope.marioAnalyzer = function () {

                var msg = undefined;
                $http.get('/api/mario')
                    .success(function (data, status, headers, config) {
                        msg = data;
                        //alert("Start analyzing Mario source code: " + msg.title + " @ " + msg.content);
                        $scope.message = "Start analyzing Mario source code...";
                    })
                    .error(function (data, status, headers, config) {
                        alert("Error when sending the AJAX request. ");
                        $scope.message = data;
                    });


            };

            $scope.pacmanAnalyzer = function () {
                var msg = undefined;
                $http.get('/api/pacman')
                    .success(function (data, status, headers, config) {
                        msg = data;
                        //alert("Start analyzing Pacman source code: " + msg.title + " @ " + msg.content);
                        $scope.message = "Start analyzing Pacman source code...";
                    })
                    .error(function (data, status, headers, config) {
                        alert("Error when sending the AJAX request. ");
                        $scope.message = data;
                    });
                //alert("Start analyzing Pacman source code...");
            };


        }]);








