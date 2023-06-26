<?php
include "includes/settings.php";
include "includes/database.php";
include  "includes/init.php";


?>


<!DOCTYPE html>
<html>
    
<head>
        
        <!-- Title -->
        <title>OutHereLive| Admin Dashboard</title>
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
                         <a href="index.html" class="logo-text"><span><font color=981E8E>OutHereLive</font></span></a>
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
                        <li><a href="Home.php"><span class="menu-icon icon-speedometer"></span><p>Home</p></a></li>
                        <li class="droplink"><a href="ManageClients.php"><span class="menu-icon icon-user"></span><p>Manage Clients</p></a></li>
                        <li class="droplink"><a href="ManageTokens.php"><span class="menu-icon icon-tag"></span><p>Manage Tokens</p><span class="arrow"></span></a></li>
						<li class="active"><a href="FailedClients.php"><span class="menu-icon icon-close"></span><p>Failed Clients</p><span class="arrow"></span></a></li>
                        <li class="droplink"><a href="/OutHere.zip"><span class="menu-icon icon-present"></span><p>Download Files</p><span class="arrow"></span></a></li>
                    </ul>
				</div>
			</div>
			
			<br>
			
			<div class="container-flid" style="width:100%">
			<div class="col-md-4 col-md-offset-4">
			<div class="row">
			<div class="panel panel-success">
			<div class="panel-heading">
			<form method="POST">
				<h3 class="panel-title">OutHereLive- Failed</h3>
			</div>
			<div class="panel-body controls no-padding">
				<div class="block">
					<table cellpadding="0" cellspacing="0" width="100%" class="table table-bordered table-striped sortable">
					<br>
					<center><button class="btn btn-block btn-success" name="clear_failedlogins">Clear Failed Logins</button></center><br>
						<thead>
							<tr>
								<th>CPUKey</th>
							</tr>
						</thead>
						<tbody>
						<?php
						$failed_result = mysqli_query($con, "SELECT * FROM `failed`");
						while($failed_rows2 = mysqli_fetch_array($failed_result))
						{
							echo'
							<tr>
							<td>
							'.$failed_rows2['cpukey'].'
							</td>
							';
						}
						?>
						</tbody>
					</table>
				</div>
			</div>
			<?php
			if(isset($_POST['clear_failedlogins']))
			{
				$clear_failed = mysqli_query($con, "TRUNCATE `failed`");
				if($clear_failed)
				{
					echo'<div class="row"><div class="col-md-12"><div class="alert alert-success">
					<button type="submit" class="close" data-dismiss="alert" aria-hidden="true">x</button>
					<strong>OK!</strong> Failed logins cleared!.
					</div></div></div>
					';
				}
				elseif(!$clear_failed)
				{
					echo'<div class="row"><div class="col-md-12"><div class="alert alert-success">
					<button type=submit" class="close" data-dismiss="alert" aria-hidden="true">x</button>
					<strong>Fail!</strong> Failed logins cant be cleared, Try Again!.
					</div></div></div>
					';
				}
			}
			?>
			</div><br />
			</form>
			</div>
			</div>
			</div>
           

			</div><!-- Main Wrapper -->
			<div class="page-footer">
				<div class="container">
					<center><p class="no-s">2016 &copy; OutHereLiveby Emodz.</p></center>
				</div>
			</div>
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