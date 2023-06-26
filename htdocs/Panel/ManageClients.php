<?php
include "includes/settings.php";
include "includes/database.php";


$clients = mysqli_query($con, "SELECT COUNT(1) FROM `consoles`");
$clients_row = mysqli_fetch_array($clients);
$clients_total = $clients_row[0];
$time_now = time("Y-m-d");

if(isset($_GET['deleteclient']))
{
    if($_GET['deleteclient'])
    {
        $id = strip_tags($_GET['deleteclient']);
        mysqli_query($con, "DELETE FROM `consoles` WHERE `uid` = '$id'") or die(mysqli_error($con));
    }
}

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
			if(isset($_POST["create"]))
			{
				$client_name = $_POST['nameVal'];
				$client_cpuk = $_POST['cpukeyVal'];
				$client_email = $_POST['emailVal'];
				$client_time = $_POST['daysVal'];
				$time_now = new datetime(date("Y-m-d"));
				$time_now->add(new DateInterval('P'.$client_time.'D'));
				$client_realtime = $time_now->format('Y-m-d') . "\n";

				$select_username_first = mysqli_query($con, "SELECT COUNT(*) FROM `consoles` WHERE `username` = '$client_name'");
				$select_cpukey_first = mysqli_query($con, "SELECT * FROM `consoles` WHERE `cpukey` = '$client_cpuk'");
					$select_cpuk_first = mysqli_num_rows($select_cpukey_first);

					if($select_cpuk_first > 0)
					{
						echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
							<center><strong>Fail!</strong> This cpukey exist in DB, please try again.</center>
						</div></div></div>';
					}
					else
					{
						$select_usermail_first = mysqli_query($con, "SELECT * FROM `consoles` WHERE `email` = '$client_email'");
						$select_email_first = mysqli_num_rows($select_usermail_first);

						if($select_email_first > 0)
						{
							echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
							<center><strong>Fail!</strong> This email exist in DB, please try again.</center>
						</div></div></div>';
						}
						else
						{
							$insert = mysqli_query($con, "INSERT INTO `consoles`(`uid`, `name`, `cpukey`, `email`, `expire`, `enabled`) VALUES ('NULL', '$client_name', '$client_cpuk', '$client_email', '$client_realtime', '1')");

					if($insert)
					{
						echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
						<center><strong>Great!</strong> '.$client_name.' successfully added.</center>
						</div></div></div>';
						echo '<meta http-equiv="refresh" content="3;url=ManageClients.php">';
					}
					elseif(!$insert)
					{
						echo '<br><button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
							<center><strong>Fail!</strong> Client not added, please try again.</center>
						</div></div></div>';
					}
					}
				}
			}
			?>
			<br>
			<div class="container-flid" style="width:100%">
			<div class="col-md-4 col-md-offset-4">
			<div class="row">
			<div class="col-xs-16">
			<div class="panel panel-success">
			<form method="POST">
			<div class="panel-heading">
				<h3 class="panel-title">OutHereLive- Add client System</h3>
			</div>
			<div class="panel-body controls no-padding">
				<div class="row-form">
				<br>
					<div class="col-md-3"><strong>Name:</strong></div>
					<div class="col-md-9"><input type="text" required class="form-control" placeholder="name" name="nameVal"/></div>
				</div>
				<div class="row-form">
					<div class="col-md-3"><strong>Email:</strong></div>
					<div class="col-md-9"><input type="email" required class="form-control" placeholder="email" name="emailVal"/></div>
				</div>
				<div class="row-form">
					<div class="col-md-3"><strong>CPU Key:</strong></div>
					<div class="col-md-9"><input type="text" required class="form-control" maxLength = "32" placeholder="cpu key" name="cpukeyVal"/></div>
				</div>
				<div class="row-form">
					<div class="col-md-3"><strong>Time:</strong></div>
					<div class="col-md-9">
						<select class="form-control" name="daysVal">
							<option value="1">1 Day</option>
							<option value="3">3 Days</option>
							<option value="7">7 Days</option>
							<option value="14">14 Days</option>
							<option value="31">31 Days</option>
							<option value="99999">Lifetime</option>
						</select>
					</div>
				</div>
			</div>
			<center><div class="panel-footer"><button class="btn btn-success" name="create">Add User</button></div></center>
			</form>
			</div>
			</div>
			</div>
			</div>
			</div>
			
			<br>
				
			<div class="container-flid" style="width:100%">
			<div class="col-md-10 col-md-offset-1">
			<div class="row">
			<div class="col-xs-14">
				<div class="panel panel-success">
					<div class="panel-heading">
						<h3 class="panel-title">OutHereLive- Users (Total: <?php echo $clients_total; ?>)</h3>
					</div>
						<div class="panel-body controls no-padding">
							<div class="block">
								<table cellpadding="0" cellspacing="0" width="100%" class="table table-bordered table-striped sortable">
									<thead>
										<tr>
											<th><center>Name</center></th>
											<th><center>Email</center></th>
											<th><center>CPU Key</center></th>
											<th><center>Salt</center></th>
											<th><center>Expires</center></th>
											<th><center>Enabled</center></th>
											<th style="width: 180px;"><center>Actions</center></th>
										</tr>
									</thead>
									<tbody>
									<tr>
									<?php
									$clients_result = mysqli_query($con, "SELECT * FROM `consoles`");
									while($clients_row2 = mysqli_fetch_array($clients_result))
									{
										echo '
										<tr>
										<td>
										<center>'.$clients_row2['name'].'</center>
										</td>
										<td>
										<center>'.$clients_row2['email'].'</center>
										</td>
										<td>
										<center>'.$clients_row2['cpukey'].'</center>
										</td>
										<td>
										<center>'.$clients_row2['salt'].'</center>
										</td>
										<td>
										<center>'.($clients_row2['expire'] >= "2290-01-01 00:00:00" ? "Lifetime" : $clients_row2['expire']).'</center>
										</td>
										<td>
										<center>'. ($clients_row2['enabled'] == "1" ? "<label class = 'label label-success'>Enabled</label>" : "<label class = 'label label-primary'>Disabled</label>") .'</label></center>
										</td>
										<td>
										<form action = "edituser.php?id='.$clients_row2['uid'].'" method="POST">
										<center><button type="submit" class="btn btn-success"><i class="fa fa-cog"></i> Delete/Edit User</button></center>
										</form>
										</td>'
										;
									}
									?>
									</tr>
									</tbody>
								</table>
							</div>
						</div>
					<br />
				</div>
            </div><br><!-- Main Wrapper -->
			<div class="page-footer">
				<div class="container">
					<center><p class="no-s">2016 &copy; OutHereLiveby Xbox Modz.</p></center>
				</div>
			</div>
        </nav>
        <div class="cd-overlay"></div>
	
<script type="text/javascript">


function populateTable(){
  
        tbl_row += "<td><input type=\"checkbox\" name=\"cb_clients[]\" value="+clientArr[8]+"></td>";
        $.each(this, function(key, value) {

		}
	  }
<script type="text/javascript">
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