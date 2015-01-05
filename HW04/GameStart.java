/**
 * A class to show the Gui from the other class.
 * 
 * @author (Paolo Chua) 
 * @version (12/29/11)
 */
import javax.swing.*;
import java.util.Random;
public class GameStart extends Thread
{
    static GameGui f;
    
    public static void main( String args[] )
    {
        f = new GameGui(700, 550);
        f.setSize( 700,550 );
        f.setTitle( "Bullet Hell" );
        f.setResizable( false );
        f.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
        f.setVisible( true );
        long startTime = System.currentTimeMillis();// start of game
        long currentTime = System.currentTimeMillis();//current time
        long patternTime = startTime;// to keep track how long each pattern lasts
        long fpsTime;
        int pattern = 0;
        Bullet temp;
        int bulletCount =0;
        int count =0;
        Random ran = new Random();
        int tempCount = 0;
        int x =50;
        boolean booltemp = true;
        (new GameStart()).start();
        while( startTime +(65*1000) > currentTime) //runs for 75 sec
        {
           fpsTime =System.currentTimeMillis();
           if(pattern == 0)
           {
               for(int i = 1; i < 34; i++)
               {
                   temp = new Bullet(350, 150, (Math.PI*i/17), true, booltemp, false);
                   f.addBul(temp);
               }
               count++;
               if( count%8 == 0)
               {
                   tempCount+= Math.PI/18;
                   count = 0;
               }
               else
               {
                   temp = new Bullet(75*count, 100, Math.PI/2, false, false, false);
                   f.addBul(temp);
               }
               currentTime = System.currentTimeMillis();
               if( patternTime + (7*1000) < currentTime )//change bullet pattern after 10 sec
               {
                   booltemp = false;
               }
               if( patternTime + (14*1000) < currentTime )//change bullet pattern after 10 sec
               {
                   pattern = 1;
                   count = 0;
                   try //give the guys a break
                   {
                       Thread.sleep( 6000 );
                    }     
                    catch (InterruptedException e) 
                    {
                        e.printStackTrace();
                        // handle the exception...        
                        // For example consider calling Thread.currentThread().interrupt(); here.
                    }
                   patternTime = System.currentTimeMillis();//update patternTime
               }
           }
           if( pattern == 1 )
           {
               int intTemp = ran.nextInt(4);
               if( intTemp == 0) 
               {
                   for(int i = 0; i < 5; i++)
                   {
                       temp = new Bullet(0, 50+(i*100), 0, false, false, true);
                       f.addBul(temp);
                   }
               }
               else
               if( intTemp == 1) 
               {
                   for(int i = 0; i < 6; i++)
                   {
                       temp = new Bullet(i*100, 25, Math.PI/2, false, false, true);
                       f.addBul(temp);
                   }
               }
               else
               if( intTemp == 2) 
               {
                   for(int i = 0; i < 5; i++)
                   {
                       temp = new Bullet(600, i*100, Math.PI, false, false, true);
                       f.addBul(temp);
                   }
               }
               else
               if( intTemp == 3) //intTemp = 3
               {
                   for(int i = 0; i < 6; i++)
                   {
                       temp = new Bullet(50+(i*100), 500, Math.PI*3/2, false, false, true);
                       f.addBul(temp);
                   }
               }
               currentTime = System.currentTimeMillis();
               if( patternTime + (4*1000) < currentTime )//change bullet pattern after 10 sec
               {
                   pattern = 2;
                   count = 0;
                   try //give the guys a break
                   {
                       Thread.sleep( 6000 );
                    }     
                    catch (InterruptedException e) 
                    {
                        e.printStackTrace();
                        // handle the exception...        
                        // For example consider calling Thread.currentThread().interrupt(); here.
                    }
                   patternTime = System.currentTimeMillis();//update patternTime
                   booltemp = true;
               }
           }
           if(pattern == 2)
           {
               
               if(booltemp )
               {
                   for(int i = 1; i < 36; i++)
                   {
                       temp = new Bullet(x, 100, (Math.PI*i/18), true, booltemp, false);
                       f.addBul(temp);
                   }
                   x=x+20;
                   if( x > 550)
                   {
                       booltemp = false;
                   }
               }
               else 
               {
                   for(int i = 1; i < 36; i++)
                   {
                       temp = new Bullet(x, 100, (Math.PI*i/18), true, booltemp, false);
                       f.addBul(temp);
                   }
                   x=x-20;
                   if( x < 50)
                        booltemp = true;
               }
           }    
           currentTime = System.currentTimeMillis();
           try 
           {
               Thread.sleep( Math.max( 500-( currentTime-fpsTime ), 0) );
           } 
           catch (InterruptedException e) 
           {
               e.printStackTrace();
               // handle the exception...        
               // For example consider calling Thread.currentThread().interrupt(); here.
           }

           currentTime = System.currentTimeMillis();
        }
        System.out.println( "you got hit " + f.num + " of times." );
        try 
        {
            Thread.sleep(4000);
        } 
        catch (InterruptedException e) 
        {
            e.printStackTrace();
            // handle the exception...        
            // For example consider calling Thread.currentThread().interrupt(); here.
        }
        System.exit(0);
    }
    
    public void run()
    {
        while( true)
        {
            long fpsTime = System.currentTimeMillis();
            f.moveBul();
            f.updateScore();
            long currentTime = System.currentTimeMillis();
            try {
               Thread.sleep( Math.max(1000/15-(currentTime-fpsTime), 0) );
            } catch (InterruptedException e) {
                e.printStackTrace();
                // handle the exception...        
                // For example consider calling Thread.currentThread().interrupt(); here.
            }
        }
    }
    
   
}

