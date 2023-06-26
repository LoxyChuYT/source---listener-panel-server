
<?php
include "includes/settings.php";
include "includes/database.php";
include  "includes/init.php";


?>
<?php
function win_sys_current_cpu_usage() {
  $cmd = 'typeperf  -sc 1  "\Processor(_Total)\% Processor Time"';
  exec($cmd, $lines, $retval);
  if($retval == 0) {
    $values = str_getcsv($lines[2]);
    return floatval($values[1]);
  } else {
    return false;
  }
}

?>


<!DOCTYPE html>
<html>
    
<head >
        
        <!-- Title -->
        <title>OutHere Live!</title>
        <meta content="width=device-width, initial-scale=1" name="viewport"/>
        <meta charset="UTF-8">
        <meta name="description" content="OutHereLiveBackstage" />
        <meta name="keywords" content="admin,Backstage,OutHereLive,Backend,Emodz" />
        <meta name="author" content="Emodz" />
        
        <!-- Styles -->
        <link href='http://fonts.googleapis.com/css?family=Ubuntu:300,400,500,700' rel='stylesheet' type='text/css'>
        <link href="assets/plugins/pace-master/themes/blue/pace-theme-flash.css" rel="stylesheet"/>
        <link href="assets/plugins/uniform/css/uniform.default.min.css" rel="stylesheet"/>
        <link href="assets/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/fontawesome/css/font-awesome.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/line-icons/simple-line-icons.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/waves/waves.min.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/switchery/switchery.min.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/3d-bold-navigation/css/style.css" rel="stylesheet" type="text/css"/>
        <link href="assets/plugins/slidepushmenus/css/component.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/weather-icons-master/css/weather-icons.min.css" rel="stylesheet" type="text/css"/>	
        <link href="assets/plugins/metrojs/MetroJs.min.css" rel="stylesheet" type="text/css"/>
        	
        <!-- Theme Styles -->
        <link href="assets/css/modern.min.css" rel="stylesheet" type="text/css"/>
        <link href="assets/css/custom.css" rel="stylesheet" type="text/css"/>
        
        <script src="assets/plugins/3d-bold-navigation/js/modernizr.js"></script>
        
				<main class="page-content content-wrap">
            <div class="navbar">
                <div class="navbar-inner container">
                    <div class="sidebar-pusher">
                    </div>
                    <div class="logo-box">
                         <a href="index.php" class="logo-text"><span><font color=981E8E>OutHere</font></span></a>
                    </div><!-- Logo Box -->
						<ul class="nav navbar-nav navbar-right">
						<li> <a href="http://logout@www.OutHerelive.tk"><font color=981E8E>Sign out</font> </a> </li>
					</ul><!-- Nav -->
				</div><!-- Top Menu -->
            </div><!-- Navbar -->
            <div class="page-sidebar sidebar horizontal-bar">
                <div class="page-sidebar-inner">
                    <ul class="menu accordion-menu">
                        <li class="nav-heading"><span>Navigation</span></li>
                        <li class="active"><a href="Home.php"><span class="menu-icon icon-speedometer"></span><p>Home</p></a></li>
                        <li class="droplink"><a href="ManageClients.php"><span class="menu-icon icon-user"></span><p>Manage Clients</p></a></li>
                        <li class="droplink"><a href="ManageTokens.php"><span class="menu-icon icon-tag"></span><p>Manage Tokens</p><span class="arrow"></span></a></li>
						<li class="droplink"><a href="FailedClients.php"><span class="menu-icon icon-close"></span><p>Failed Clients</p><span class="arrow"></span></a></li>
                        <li class="droplink"><a href="/OutHere.zip"><span class="menu-icon icon-present"></span><p>Download Files</p><span class="arrow"></span></a></li>
                    </ul>
				</div>
			</div>
			           
			<div class="summary">
				<center>   <h4 class="title">Server CPU Usage</h4> </center>
                <div class="info">
					<center> <strong class="amount"><?php echo win_sys_current_cpu_usage(); ?>%</strong> <center>
                </div>
			</div>
			<br>
            
		<section class="container"> 
		<div class="panel panel-success">
		<header class="panel-heading">
		<form method="POST">
		<h2 class="panel-title">OutHereLive- Server Console</h2>
		</header>
		<div class="panel-body">
		<textarea class="form-control" style="margin-top: 0px; margin-bottom: 0px; height: 480px; cursor: auto;" readonly>
		<?php echo file_get_contents("C:\Users\Administrator\Desktop\Debug\console.log"); ?>
		</textarea>
		</form>
		</div>
		</section>
		
        <div class="page-footer">
			<div class="container">
				<center><p class="no-s">2016 &copy; OutHereby Xbox Modz.</p></center>
			</div>
		</div>
      
        </nav>
        <div class="cd-overlay"></div>
	

        <!-- Javascripts -->
        <script src="assets/plugins/jquery/jquery-2.1.4.min.js"></script>
        <script src="assets/plugins/jquery-ui/jquery-ui.min.js"></script>
        <script src="assets/plugins/pace-master/pace.min.js"></script>
        <script src="assets/plugins/jquery-blockui/jquery.blockui.js"></script>
        <script src="assets/plugins/bootstrap/js/bootstrap.min.js"></script>
        <script src="assets/plugins/jquery-slimscroll/jquery.slimscroll.min.js"></script>
        <script src="assets/plugins/switchery/switchery.min.js"></script>
        <script src="assets/plugins/uniform/jquery.uniform.min.js"></script>
        <script src="assets/plugins/classie/classie.js"></script>
        <script src="assets/plugins/waves/waves.min.js"></script>
        <script src="assets/plugins/3d-bold-navigation/js/main.js"></script>
        <script src="assets/plugins/waypoints/jquery.waypoints.min.js"></script>
        <script src="assets/plugins/jquery-counterup/jquery.counterup.min.js"></script>
        <script src="assets/plugins/toastr/toastr.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.time.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.symbol.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.resize.min.js"></script>
        <script src="assets/plugins/flot/jquery.flot.tooltip.min.js"></script>
        <script src="assets/plugins/curvedlines/curvedLines.js"></script>
        <script src="assets/plugins/metrojs/MetroJs.min.js"></script>
    </body>
</html>