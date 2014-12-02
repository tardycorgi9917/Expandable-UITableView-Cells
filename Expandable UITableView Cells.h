//
//  Expandable UITableView Cells.h
//  GIT
//
//  Created by thomas karatzas on 12/2/14.
//  Copyright (c) 2014 Super Smart Apps. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Expandable_UITableView_Cells ()
{
    NSMutableArray *amounts;
    NSMutableArray *names;
    NSMutableArray *explanation;
    NSMutableArray *dates;
    NSMutableArray *profID;
    int selectedIndex; }
@end
@implementation Expandable_UITableView_Cells : NSObject

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if(selectedIndex == indexPath.row)
    {
        selectedIndex = -1;
        [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
        
        return;
    }
    
    //First we check if a cell is already expanded.
    //If it is we want to minimize make sure it is reloaded to minimize it back
    if(selectedIndex >= 0)
    {
        NSIndexPath *previousPath = [NSIndexPath indexPathForRow:selectedIndex inSection:0];
        selectedIndex = indexPath.row;
        [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObject:previousPath] withRowAnimation:UITableViewRowAnimationFade];
    }
    
    //Finally set the selected index to the new selection and reload it to expand
    selectedIndex = indexPath.row;
    [tableView reloadRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
}





- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    /*if([indexPath isEqual:[tableView indexPathForSelectedRow]]) {
     return 150;
     }
     
     return 75;*/
    if(selectedIndex == indexPath.row)
    {
        return [self getLabelHeightForIndex:indexPath.row] + COMMENT_LABEL_PADDING * 2;
    }
    else {
        return COMMENT_LABEL_MIN_HEIGHT + COMMENT_LABEL_PADDING * 2;
    }
    
}
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


- (void)viewDidLoad {
    self.title = @"My Costs";
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) viewDidAppear:(BOOL)animated {
    _myCosts=[Data shared].mesCosts;
    //NSLog(@"In MyCostsViewController %@", [Data shared].mesCosts);
    amounts= [[NSMutableArray alloc] init];
    names= [[NSMutableArray alloc] init];
    explanation =[[NSMutableArray alloc]init];
    dates=[[NSMutableArray alloc]init];
    profID=[[NSMutableArray alloc]init];
    for (Cost *ind in _myCosts)
    {
        
        //NSLog(@"Loading costs");
        [amounts addObject: ind.amount];
        [names addObject:ind.who];
        [explanation addObject:ind.why];
        [dates addObject:ind.when];
        if(ind.fbID)
        {
            
            [profID addObject:ind.fbID];
        }
        else
        {
            [profID addObject:@"NoID"];
        }
    }
    
    [self.tableView reloadData];
    [super viewDidAppear:animated];
}



- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSLog(@"%lu rows", (unsigned long)[names count]);
    return [names count];
    
}

-(BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
}
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        
        PFQuery *delete= [PFQuery queryWithClassName:@"MesCosts"];
        [delete whereKey:@"name" equalTo:[names objectAtIndex:indexPath.row]];
        [delete whereKey:@"why" equalTo:[explanation objectAtIndex:indexPath.row]];
        [delete getFirstObjectInBackgroundWithBlock:^(PFObject *object, NSError *error) {
            [object deleteInBackground];
        }];
        
        
        
        
        
        [self.myCosts removeObjectAtIndex:indexPath.row];
        
        [amounts removeObjectAtIndex:indexPath.row];
        [names removeObjectAtIndex:indexPath.row];
        [explanation removeObjectAtIndex:indexPath.row];
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationMiddle];
    }
    
}

-(CGFloat)getLabelHeightForIndex:(NSInteger)index
{
    
    
    //CGSize labelHeighSize = 200
    return 150;
    
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    
    
    static NSString *CellIdentifier = @"customCell";
    
    CustomTableCell *cell = (CustomTableCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        //cell = [[[exerciseListUITableCell alloc] init] autorelease];
        
        NSArray * topLevelObjects = [[NSBundle mainBundle] loadNibNamed:@"CustomCell" owner:self options:nil];
        
        for(id currentObject in topLevelObjects)
        {
            if([currentObject isKindOfClass:[UITableViewCell class]])
            {
                cell = (CustomTableCell *)currentObject;
                break;
            }
        }
    }
    
    //If this is the selected index then calculate the height of the cell based on the amount of text we have
    if(selectedIndex == indexPath.row)
    {
        CGFloat labelHeight = [self getLabelHeightForIndex:indexPath.row];
        
        cell.exp.frame = CGRectMake(cell.exp.frame.origin.x,
                                    cell.exp.frame.origin.y,
                                    cell.exp.frame.size.width,
                                    labelHeight);
        cell.exp.hidden=NO;
        cell.date.hidden=NO;
        
    }
    else {
        
        //Otherwise just return the minimum height for the label.
        cell.exp.frame = CGRectMake(cell.exp.frame.origin.x,
                                    cell.exp.frame.origin.y,
                                    cell.exp.frame.size.width,
                                    COMMENT_LABEL_MIN_HEIGHT);
        cell.exp.hidden=YES;
        cell.date.hidden=YES;
        
        
    }
    
    //cell.commentTextLabel.font = [UIFont fontWithName:@"Helvetica" size:14.0f];
    cell.exp.text = [NSString stringWithFormat:@"Explanation: %@",[explanation objectAtIndex:indexPath.row]];
    cell.qui.text=[names objectAtIndex:indexPath.row];
    cell.combien.text=[NSString stringWithFormat:@"$%@",[amounts objectAtIndex:indexPath.row]];
    cell.date.text=[NSString stringWithFormat:@"Added: %@", [dates objectAtIndex:indexPath.row]];
    if(![[profID objectAtIndex:indexPath.row] isEqualToString:@"NoID" ])
    {
        cell.addprof.profileID=[profID objectAtIndex:indexPath.row];
    }
    return cell;
    
}
/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */
@end
