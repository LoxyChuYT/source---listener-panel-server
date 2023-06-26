
<?php
include "includes/settings.php";
include "includes/database.php";
include  "includes/init.php";

if(!isset($_GET['id'])){
		header('location: index.php');
	}
	$id = $_GET['id'];
	$GetInfo = mysqli_query($con, "SELECT * FROM `consoles` WHERE `uid` = '$id' LIMIT 1");
	$clientinfo = mysqli_fetch_array($GetInfo);
	$client_name = $clientinfo['name'];
	$client_email = $clientinfo['email'];
	$client_cpuk = $clientinfo['cpukey'];
	$client_expire = $clientinfo['expire'];
	$client_enabled = $clientinfo['enabled'];
	$client_expire2 = strtotime($client_expire);
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
                        <li class="active"><a href="ManageClients.php"><span class="menu-icon icon-user"></span><p>Manage Clients</p></a></li>
                        <li class="droplink"><a href="ManageTokens.php"><span class="menu-icon icon-tag"></span><p>Manage Tokens</p><span class="arrow"></span></a></li>
						<li class="droplink"><a href="FailedClients.php"><span class="menu-icon icon-close"></span><p>Failed Clients</p><span class="arrow"></span></a></li>
                        <li class="droplink"><a href="/OutHere.zip"><span class="menu-icon icon-present"></span><p>Download Files</p><span class="arrow"></span></a></li>
                    </ul>
				</div>
			</div>
		    <?php
			if(isset($_POST['upduser']))
			{
				$clientname = $_POST['nameVal'];
				$clientcpuk = $_POST['cpukeyVal'];
				$clientemail = $_POST['emailVal'];
				$clienttime = $_POST['timeVal'];
				$update_now = mysqli_query($con, "UPDATE `consoles` SET `name`='$clientname',`cpukey`='$clientcpuk', `email`='$clientemail', `expire`='$clienttime' WHERE `cpukey` = '$client_cpuk'");
				if($update_now)
				{
					echo'<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">x</button>
					<center><strong></strong> Client successfully updated.</center>
					</div></div></div>';
					echo '<meta http-equiv="refresh" content="1;url="ManageClients.php">';
				}
				elseif(!$update_now)
				{
					echo'<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">x</button>
					<center><strong></strong> Client failed to update.</center>
					</div></div></div>
					';
				}
			}
			?>
			<?php
			if(isset($_POST['deleteuser']))
			{
				$deletenow = mysqli_query($con, "DELETE FROM `consoles` WHERE `cpukey` = '$client_cpuk'");
				if($deletenow)
				{
					echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
						<center><strong></strong> Client successfully deleted.</center>
						</div></div></div>';
					echo '<meta http-equiv="refresh" content="1;url=ManageClients.php">';
				}
				else
				{
					echo '		<button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
									<strong></strong> Client failed to delete!.
								</div></div></div>';					        	
				}
			}
			?>
			<?php
			if(isset($_POST['statususer']))
			{
				if($client_enabled == 0)
				{
					$enable = mysqli_query($con, "UPDATE `consoles` SET `enabled`='1' WHERE `cpukey` = '$client_cpuk'");
					if($enable)
					{
							echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
									<center><strong></strong> Client successfully enabled!.</center>
								</div></div></div>';
								echo '<meta http-equiv="refresh" content="2">';
					}
					else
					{
							echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
									<center><strong></strong> Client not enabled!.</center>
								</div></div></div>';
					}
				}
				elseif($client_enabled == 1)
				{
					$disable = mysqli_query($con, "UPDATE `consoles` SET `enabled`='0' WHERE `cpukey` = '$client_cpuk'");

					if($disable)
					{
							echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
									<center><strong></strong> Client successfully disabled!.</center>
								</div></div></div>';
								echo '<meta http-equiv="refresh" content="2">';
					}
					else
					{
							echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
									<center><strong></strong> Client failed to disable!</center>
								</div></div></div>';
					}
				}
			}
			?>
			<br><br>
			<div class="col-md-4 col-md-offset-4">
				<div class="center-row">		
					<div class="panel panel-success">
						<form method="POST">
							<div class="panel-heading">
								<h3 class="panel-title">Edit User</h3>
							</div>
						<div class="panel-body controls no-padding">
						<div>
							<div>
								<div>
								<br>
									<div> <strong>Name :</strong> </div>
									<div> <input type="text" required class="form-control" placeholder="Name" value = "<?php echo $client_name; ?>" name="nameVal"/></div>
									<br>
									<div>  <strong>CPU Key :</strong></div>
									<div> <input type="text" required class="form-control" maxLength = "32" value = "<?php echo $client_cpuk; ?>" placeholder="CPUKey" name="cpukeyVal"/></div>
									<div>  <strong>Email :</strong></div>
									<div> <input type="text" required class="form-control" maxLength = "32" value = "<?php echo $client_email; ?>" placeholder="email" name="emailVal"/></div>
									<div> <strong>Time : </strong></div>
									<input type="text" required class="form-control datepicker" value = "<?php echo $client_expire; ?>" name="timeVal"/></date>
								</div>
							</div>
						</div>
						<div class="panel-footer">
							<div>
								<div>
									<br>
										<center><button class="btn btn-success" name="upduser">Update Info</button>
										<button class="btn btn-success" name="deleteuser">Delete</button>
										<?php
										if($client_enabled == 1)
										{
											echo '<button class="btn btn-success" name="statususer">Disable</button>';
										}
										elseif($client_enabled == 0)
										{
											echo '<button class="btn btn-success" name="statususer">Enable</button>';
										}
										?></center>
										<br>
									</div>
						</form>
							</div>
						</div>
					</div>
				</div>
			</div><!-- Main Wrapper -->
			</br>
			
			</div><!-- Main Wrapper -->
                <div class="page-footer">
                    <div class="container">
                        <center><p class="no-s">2016 &copy; OutHereLiveby Emodz.</p></center>
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