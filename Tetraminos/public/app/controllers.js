'use strict';

/* Controllers */

angular.module('myApp.controllers', [])
  .controller('AppCtrl', ['$scope', '$location', '$http', '$routeParams',
   function ($scope, $location, $http, $routeParams) {

       $scope.cc = "The code analyzer area.";



       $scope.marioAnalyzer = function () {
           alert("Start analyzing Mario source code...");
       };

        $scope.pacmanAnalyzer = function () {
           alert("Start analyzing Pacman source code...");
       };


   	
   }]);








